#ifndef __CF_SOCKET_H__
#define __CF_SOCKET_H__

#include <cf/types.h>
#include <sys/socket.h>


extern const cf_int_t CF_SOCK_AF_UNSPEC;
extern const cf_int_t CF_SOCK_AF_LOCAL;
extern const cf_int_t CF_SOCK_AF_INET;
extern const cf_int_t CF_SOCK_AF_INET6;

extern const cf_int_t CF_SOCK_STREAM;
extern const cf_int_t CF_SOCK_DGRAM;
extern const cf_int_t CF_SOCK_RAW;

extern const cf_int_t CF_SOCK_PROTO_TCP;
extern const cf_int_t CF_SOCK_PROTO_UDP;

typedef union {
    cf_uint32_t addr_int;
    cf_uint8_t addr_byte[4];
} cf_sockaddr_inet_t;

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
typedef cf_uint_t cf_sock_len_t;

cf_uint16_t cf_sock_ntohs(uint16_t n);
cf_uint16_t cf_sock_htons(uint16_t n);
cf_uint32_t cf_sock_ntohl(uint32_t n);
cf_uint32_t cf_sock_htonl(cf_uint32_t n);
cf_char_t* cf_sock_inet_ntoa(cf_sockaddr_inet_t in);
cf_errno_t cf_sock_inet_aton(const cf_char_t* s, cf_sockaddr_inet_t* addr);

cf_errno_t cf_sock_create(cf_socket_t* sock, cf_int_t family, cf_int_t type, cf_int_t protocol);
cf_errno_t cf_sock_close(cf_socket_t sock);
cf_errno_t cf_sock_bind(cf_socket_t sock, cf_sockaddr_t* sa, cf_sock_len_t addrlen);
cf_errno_t cf_sock_listen(cf_socket_t sock, cf_uint_t backlog);
cf_errno_t cf_sock_accept(cf_socket_t sock, cf_socket_t* new_sock, cf_sockaddr_t* addr, cf_sock_len_t* addrlen);
cf_errno_t cf_sock_connect(cf_socket_t sock, cf_sockaddr_t* sa, cf_int_t namelen);
cf_errno_t cf_sock_send(cf_socket_t sock, cf_void_t* buff, cf_sock_len_t* len, cf_uint_t flags);
cf_errno_t cf_sock_recv(cf_socket_t sock, cf_void_t* buff, cf_sock_len_t* len, cf_uint_t flags);
cf_errno_t cf_sock_sendto(cf_socket_t sock, cf_void_t* buff, cf_sock_len_t* len, cf_uint_t flags, cf_sockaddr_t* sa, cf_sock_len_t addrlen);
cf_errno_t cf_sock_recvfrom(cf_socket_t sock, cf_void_t* buff, cf_sock_len_t* len, cf_uint_t flags, cf_sockaddr_t* from, cf_sock_len_t* fromlen);



#endif /* __CF_SOCKET_H__ */
