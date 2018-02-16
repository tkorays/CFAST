#include <cf/socket.h>
#include <cf/err.h>
#include <cf/memory.h>
#include <cf/thread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void error_exit(cf_char_t* msg, cf_errno_t status) {
    cf_char_t buff[1024] = {0};
    cf_err_strerr(status, buff, 1024);

    printf("[ERROR]: %s, %s\n", msg, buff);
    exit(1);
}

cf_void_t* sock_accept_process(cf_void_t* arg) {
    cf_socket_t tid = *(cf_socket_t*)arg;
    cf_char_t recvbuf[1024] = {0};
    cf_size_t len = 1024;
    printf("enter...\n");
    cf_thread_detach(cf_thread_self());
    cf_sock_recv(tid, recvbuf, &len, 0);
    printf("process sock connect ok, recv: %s\n", recvbuf);

    cf_sock_close(tid);
    return CF_NULL_PTR;
}

int main() {
    cf_socket_t sock = CF_SOCK_INVALID;
    cf_socket_t sockcli = CF_SOCK_INVALID;
    cf_errno_t status;
    cf_sock_len_t len;
    cf_sockaddr_in_t cliaddr, servaddr;

    cf_thread_t tid;
    cf_thread_attr_t tattr;

    status = cf_sock_create(&sock, CF_SOCK_AF_INET, CF_SOCK_STREAM, CF_SOCK_PROTO_AUTO);
    if(status != CF_OK) error_exit("cf_sock_create fail!", status);
    cf_memset_s(&servaddr, sizeof(cf_sockaddr_in_t), 0, sizeof(cf_sockaddr_in_t));
    servaddr.sin_family = CF_SOCK_AF_INET;
    servaddr.sin_addr.s_addr = cf_sock_htonl(0);
    servaddr.sin_port = cf_sock_htons(9876);

    status = cf_sock_bind(sock, (cf_sockaddr_t*)&servaddr, sizeof(servaddr));
    if(status != CF_OK) error_exit("cf_sock_bind fail!", status);

    status = cf_sock_listen(sock, 5);
    if(status != CF_OK) error_exit("cf_sock_listen fail!", status);

    printf("listening...\n");
    for(;;) {
        len = sizeof(cliaddr);
        status = cf_sock_accept(sock, &sockcli, (cf_sockaddr_t*)&cliaddr, &len);
        if(status != CF_OK) error_exit("cf_sock_accept fail!", status);

        printf("accept!\n");
        //cf_memset_s(&tattr, sizeof(cf_thread_attr_t), 0, sizeof(cf_thread_attr_t));
        cf_thread_attr_init(&tattr);
        status = cf_thread_create(&tid, &tattr, sock_accept_process, (cf_void_t*)&sockcli);
        //cf_sock_close(sockcli);
    }
    return 0;
}