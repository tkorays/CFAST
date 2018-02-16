#include <cf/socket.h>
#include <cf/memory.h>
#include <cf/err.h>
#include <stdio.h>
#include <stdlib.h>

void error_exit(cf_char_t* msg, cf_errno_t status) {
    cf_char_t buff[1024] = {0};
    cf_err_strerr(status, buff, 1024);

    printf("[ERROR]: %s, %s", msg, buff);
    exit(1);
}

int main() {
    cf_socket_t sock;
    cf_sockaddr_in_t addr;
    cf_errno_t status;
    cf_size_t len;

    cf_memset_s(&addr, sizeof(cf_sockaddr_t), 0, sizeof(cf_sockaddr_t));
    status = cf_sock_create(&sock, CF_SOCK_AF_INET, CF_SOCK_STREAM, CF_SOCK_PROTO_AUTO);
    if(status != CF_OK) error_exit("cf_sock_create fail!", status);

    addr.sin_family = CF_SOCK_AF_INET;
    addr.sin_port = cf_sock_htons(9876);
    cf_sock_inet_aton("127.0.0.1", &addr.sin_addr);

    status = cf_sock_connect(sock, (cf_sockaddr_t*)&addr, sizeof(addr));
    if(status != CF_OK) error_exit("cf_sock_connect fail!", status);

    len = 5;
    cf_sock_send(sock, "hello", &len, 0);
    printf("send ok..\n");
    cf_sock_close(sock);
    return 0;
}