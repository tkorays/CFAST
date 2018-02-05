#ifndef __CF_SOCKET_H__
#define __CF_SOCKET_H__

#include <cf/types.h>
#include <sys/socket.h>

/**
 * address family
 */
typedef enum {
    CF_SOCK_AF_UNSPEC   = 0,
    CF_SOCK_AF_LOCAL    = 1,
    CF_SOCK_AF_INET     = 2,
    CF_SOCK_AF_INET6    = 3,
} cf_sock_af_t;

/**
 * socket type
 */
typedef enum {
    CF_SOCK_STREAM  = 0,
    CF_SOCK_DGRAM   = 1,
    CF_SOCK_RAW     = 2,
} cf_sock_type_t;

/**
 * socket protocol type
 */
typedef enum {
    CF_SOCK_PROTO_TCP   = 0,
    CF_SOCK_PROTO_UDP   = 1,
    CF_SOCK_PROTO_SCTP  = 2,
} cf_sock_proto_t;

typedef struct {
    cf_uint8_t  sa_len;
    cf_uint8_t  sa_family;
    cf_char_t   sa_data[14];
} cf_sockaddr_t;

typedef struct{
    union {
        cf_uint32_t __uint32;
        cf_uint8_t  __u_addr8[4];
    } __in_addr;
} cf_in_addr_t;

typedef struct {
    cf_uint8_t      sin_len;
    cf_uint8_t      sin_family;
    cf_uint16_t     sin_port;
    cf_in_addr_t    sin_addr;
    char		    sin_zero[8];
} cf_sockaddr_in_t;

typedef struct  {
    union {
        cf_uint8_t   __u6_addr8[16];
        cf_uint16_t  __u6_addr16[8];
        cf_uint32_t  __u6_addr32[4];
    } __u6_addr;			/* 128-bit IP6 address */
} cf_in6_addr_t;

typedef struct {
    cf_uint8_t  sin6_family;   /* AF_INET6 */
    cf_uint16_t sin6_port;     /* port number */
    cf_uint32_t sin6_flowinfo; /* IPv6 flow information */
    cf_in6_addr_t sin6_addr;     /* IPv6 address */
    uint32_t        sin6_scope_id; /* Scope ID (new in 2.4) */
} cf_sockaddr_in6_t;

typedef cf_int_t  cf_socket_t;

typedef struct cf_sockaddr_s cf_sockaddr_t;

cf_errno_t cf_socket_create(cf_socket_t* sock, cf_sock_af_t family, cf_sock_type_t type, cf_sock_proto_t protocol);
cf_errno_t cf_socket_close(cf_socket_t sock);
cf_errno_t cf_socket_bind(cf_socket_t sock, cf_sockaddr_t* sa);
cf_errno_t cf_socket_listen(cf_socket_t sock, cf_uint_t backlog);
cf_errno_t cf_socket_accept(cf_socket_t sock, cf_socket_t* new_sock);
cf_errno_t cf_socket_connect(cf_socket_t sock, cf_sockaddr_t* sa);
cf_errno_t cf_socket_send(cf_socket_t sock, cf_void_t* buff, cf_size_t len);
cf_errno_t cf_socket_recv(cf_socket_t sock, cf_void_t* buff, cf_size_t* len);
cf_errno_t cf_socket_sendto();
cf_errno_t cf_socket_recvfrom();

#endif /* __CF_SOCKET_H__ */
