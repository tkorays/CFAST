#ifndef __CF_SOCKET_H__
#define __CF_SOCKET_H__

#include <cf/types.h>

typedef enum {
    CF_SOCK_PROTO_TCP = 0,
    CF_SOCK_PROTO_UDP = 1,
    CF_SOCK_PROTO_SCTP =2
};

#define cf_inet_addr inet_addr

#define CF_INADDR_NONE ((cf_uint32_t) 0xffffffff)
typedef struct {
    cf_uint32_t s_addr;
} cf_sock_in_addr_t;

typedef struct cf_socket_s cf_socket_t;
typedef struct cf_sockaddr_s cf_sockaddr_t;

cf_errno_t cf_socket_create(cf_socket_t** sock, cf_int_t family, cf_int_t type, cf_int_t protocol);
cf_errno_t cf_socket_close(cf_socket_t* sock);
cf_errno_t cf_socket_bind(cf_socket_t* sock, cf_sockaddr_t* sa);
cf_errno_t cf_socket_listen(cf_socket_t* sock, cf_uint_t backlog);
cf_errno_t cf_socket_accept(cf_socket_t* sock, cf_socket_t** new_sock);
cf_errno_t cf_socket_connect(cf_socket_t* sock, cf_sockaddr_t* sa);
cf_errno_t cf_socket_send(cf_socket_t* sock, cf_void_t* buff, cf_size_t len);
cf_errno_t cf_socket_recv(cf_socket_t* sock, cf_void_t* buff, cf_size_t* len);
cf_errno_t cf_socket_sendto();
cf_errno_t cf_socket_recvfrom();

#endif /* __CF_SOCKET_H__ */
