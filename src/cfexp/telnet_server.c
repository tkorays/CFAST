#include <cfexp/telnet_server.h>
#include <cf/socket.h>
#include <cf/err.h>
#include <cf/str.h>
#include <cf/select.h>
#include <cf/memory.h>

#include <stdio.h>

CF_THREAD_DEF_PROC(telnet_proc_func, arg) {
    cf_int_t maxfd = 0;
    cf_fdset_t rfd;
    cf_fdset_t wfd;
    cf_telnet_server_t* serv = (cf_telnet_server_t*)arg;
    cf_errno_t status;

    if(!serv || !serv->sock) return CF_THREAD_RET(CF_NOK);

    cf_thread_detach(cf_thread_self());

    // 远端socket读取
    cf_fd_zero(&rfd);
    cf_fd_set(serv->sock, &rfd);
    maxfd = CF_MAX2(maxfd, serv->sock);

    for(;;) {
        cf_fd_zero(&rfd);
        cf_fd_set(serv->sock, &rfd);

        status = cf_select(maxfd+1, &rfd, CF_NULL_PTR, CF_NULL_PTR, CF_NULL_PTR);
        if(status != CF_OK) {
            continue;
        }

        if(cf_fd_isset(serv->sock, &rfd)) {
            printf("accept!!!!!\n");
        }

    }
    
    return CF_THREAD_RET(0);
}

CF_DECLARE(cf_errno_t) cf_telnet_server_create(
    cf_telnet_server_t* serv, 
    const cf_char_t* host, 
    cf_uint16_t port
) {
    cf_sockaddr_in_t servaddr;
    cf_thread_attr_t attr;
    if(!serv || !host || !port) return CF_NOK;

    /* 服务器端socket创建 */
    if(CF_OK != cf_sock_create(&serv->sock, CF_SOCK_AF_INET, CF_SOCK_STREAM, CF_SOCK_PROTO_AUTO)) {
        return CF_NOK;
    }
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

    /* 新建一个线程处理连接和输入输出任务 */
    cf_thread_attr_init(&attr);
    if(CF_OK != cf_thread_create(&serv->thr, &attr, telnet_proc_func, 0)) {
        cf_sock_close(serv->sock);
    }

    return CF_OK;
}
