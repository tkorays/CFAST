#include <cfexp/telnet_server.h>
#include <cf/socket.h>
#include <cf/err.h>
#include <cf/str.h>
#include <cf/select.h>
#include <cf/memory.h>
#include <stdarg.h>

#include <stdio.h>

typedef struct {
    cf_telnet_server_t* serv;
    cf_int_t idx;
} cli_session_t;

static cf_bool_t _add_client(cf_telnet_server_t* serv, 
cf_socket_t sock,
cf_sockaddr_in_t* addr) {
    cf_uint_t i;
    for(i = 0; i < CF_TELNET_SERV_MAX_USER; i++) {
        if(!serv->client[i].inuse) {
            serv->client[i].sock = sock;
            cf_memcpy_s(&serv->client[i].addr, sizeof(cf_sockaddr_in_t), addr, sizeof(cf_sockaddr_in_t));
            serv->client[i].inuse = CF_TRUE;
            return CF_TRUE;
        }
    }
    return CF_FALSE;
}

CF_THREAD_DEF_PROC(telnet_proc_func, arg) {
    cf_int_t maxfd = 0;
    cf_fdset_t rfd;
    cf_fdset_t wfd;
    cf_telnet_server_t* serv = (cf_telnet_server_t*)arg;
    cf_errno_t status;
    cf_socket_t nsock;
    cf_sockaddr_in_t addr;
    cf_sock_len_t len = sizeof(addr);
    cf_uint_t i, j;
    cf_char_t buff[1024] = {0};
    cf_size_t bsize;
    cf_cliarg_t cliarg;
    cli_session_t sess;

    if(!serv || !serv->sock) return CF_THREAD_RET(CF_NOK);
    cf_thread_detach(cf_thread_self());

    // 远端socket读取
    cf_fd_zero(&rfd);
    cf_fd_set(serv->sock, &rfd);

    while(!serv->stop) {
        cf_fd_zero(&rfd);
        cf_fd_set(serv->sock, &rfd);
        maxfd = CF_MAX2(maxfd, serv->sock);
        for(i = 0; i < CF_TELNET_SERV_MAX_USER; i++) {
            if(serv->client[i].inuse) {
                cf_fd_set(serv->client[i].sock, &rfd);
                maxfd = CF_MAX2(maxfd, serv->client[i].sock);
            }
        }
        
        status = cf_select(maxfd+1, &rfd, CF_NULL_PTR, CF_NULL_PTR, CF_NULL_PTR);
        if(status != CF_OK) {
            continue;
        }
        //printf("select ok\n");

        // server的socket表示是连接请求
        if(cf_fd_isset(serv->sock, &rfd)) {
            if(CF_OK == cf_sock_accept(serv->sock, &nsock, (cf_sockaddr_t*)&addr, &len)) {
                // 将新的socket加入socket集合
                if(!_add_client(serv, nsock, &addr)) {
                    //printf("connetion is up to 5.\n");
                    cf_sock_close(nsock);
                } else {
                    //printf("add connection.\n");
                    cf_sock_set_nonblock(nsock, CF_TRUE);
                }
            }
        }

        for(i = 0; i < CF_TELNET_SERV_MAX_USER; i++) {
            if(serv->client[i].inuse) {
                bsize = 1024;
                if(CF_OK == cf_sock_recv(serv->client[i].sock, buff, &bsize, 0)) {
                    j = 0;
                    while(buff[j] && buff[j] != '\n' && buff[j] != '\r') ++j;
                    buff[j] = '\0';
                    //printf("recv: %s\n", buff);
                    cf_cli_parse_arg(buff, &cliarg);
                    sess.serv = serv;
                    sess.idx = i;
                    cf_cli_input(&serv->cli, &sess, cliarg.argc, cliarg.argv);
                    cf_cli_destroy_arg(&cliarg);

                    bsize = 5;
                    cf_sock_send(serv->client[i].sock, "\r\n$ ", &bsize, 0);
                }
            }
        }

    }

    cf_sock_shutdown(serv->sock, CF_SOCK_SHUTDOWN_BOTH);
    return CF_THREAD_RET(0);
}

cf_void_t _cli_sock_send(cf_void_t* p, const cf_char_t* s, ...) {
    cli_session_t *sess = (cli_session_t*)p;
    cf_socket_t sock = 0;
    cf_char_t buf[1024] = {0};
    va_list args;
    cf_size_t len;

    if(!p) return ;
    sock = sess->serv->client[sess->idx].sock;
    if(!sock) return ;

    va_start(args, s);
    vsprintf(buf, s, args);
    va_end(args);
    len = cf_strlen(buf) + 1;
    cf_sock_send(sock, buf, &len, 0);
}

CF_CLI_CMD_FUNC(cli_quit_func, cli, sess, argc, argv) {
    cli_session_t *s = (cli_session_t*)sess;
    cf_socket_t sock = s->serv->client[s->idx].sock;
    int i;
    if(!cli || !sess) return CF_NOK;
    cf_sock_shutdown(sock, CF_SOCK_SHUTDOWN_BOTH);
    s->serv->client[s->idx].sock = 0;
    s->serv->client[s->idx].inuse = CF_FALSE;
    return CF_OK;
}
CF_DECLARE(cf_errno_t) cf_telnet_server_create(
    cf_telnet_server_t* serv, 
    const cf_char_t* host, 
    cf_uint16_t port
) {
    cf_sockaddr_in_t servaddr;
    cf_thread_attr_t attr;
    cf_cli_cfg_t clicfg;
    if(!serv || !host || !port) return CF_NOK;

    cf_memset_s(serv, sizeof(cf_telnet_server_t), 0, sizeof(cf_telnet_server_t));

    cf_cli_cfg_default(&clicfg);
    clicfg.output = _cli_sock_send; // 设置服务器通过socket输出到客户端
    if(CF_OK != cf_cli_init(&serv->cli, &clicfg)) {
        return CF_NOK;
    }
    cf_cli_register(&serv->cli, "quit", "quit the program", cli_quit_func);

    /* 服务器端socket创建 */
    if(CF_OK != cf_sock_create(&serv->sock, CF_SOCK_AF_INET, CF_SOCK_STREAM, CF_SOCK_PROTO_AUTO)) {
        return CF_NOK;
    }
    cf_sock_set_nonblock(serv->sock, CF_TRUE);
    cf_memset_s(&servaddr, sizeof(cf_sockaddr_in_t), 0, sizeof(cf_sockaddr_in_t));
    servaddr.sin_family = CF_SOCK_AF_INET;
    cf_sock_pton(CF_SOCK_AF_INET, host, &servaddr.sin_addr);
    servaddr.sin_port = cf_sock_htons(port);
    if(CF_OK != cf_sock_bind(serv->sock, (cf_sockaddr_t*)(&servaddr), sizeof(servaddr))) {
        return CF_NOK;
    }
    cf_strcpy_s(serv->host, sizeof(serv->host), host);
    serv->port = port;
    if(CF_OK != cf_sock_listen(serv->sock, CF_TELNET_SERV_MAX_USER)) {
        return CF_NOK;
    }

    serv->stop = CF_FALSE;
    /* 新建一个线程处理连接和输入输出任务 */
    cf_thread_attr_init(&attr);
    if(CF_OK != cf_thread_create(&serv->thr, &attr, telnet_proc_func, serv)) {
        cf_sock_close(serv->sock);
    }

    return CF_OK;
}

CF_DECLARE(cf_errno_t) cf_telnet_server_destroy(
    cf_telnet_server_t* serv
) {
    if(serv) {
        serv->stop = CF_TRUE;
    }
    return CF_OK;
}