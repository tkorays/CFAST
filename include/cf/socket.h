/****************************************************
 * Copyright (c) 2018, tkorays <tkorays@hotmail.com>
 * All rights reserved.
 *
 * @file    socket.h
 * @brief   socket.
 * @version 1.0
 * @author  tkorays<tkorays@hotmail.com>
 * @date    2018-2-10
 ****************************************************/
#ifndef __CF_SOCKET_H__
#define __CF_SOCKET_H__

#include <cf/types.h>

#ifdef CF_OS_WIN
#pragma comment(lib, "WS2_32.lib")
#endif

CF_DECLS_BEGIN

#define CF_SOCK_INVALID 0


extern const cf_int_t CF_SOCK_AF_UNSPEC;
extern const cf_int_t CF_SOCK_AF_LOCAL;
extern const cf_int_t CF_SOCK_AF_INET;
extern const cf_int_t CF_SOCK_AF_INET6;

extern const cf_int_t CF_SOCK_STREAM;
extern const cf_int_t CF_SOCK_DGRAM;
extern const cf_int_t CF_SOCK_RAW;

extern const cf_int_t CF_SOCK_PROTO_AUTO;
extern const cf_int_t CF_SOCK_PROTO_TCP;
extern const cf_int_t CF_SOCK_PROTO_UDP;

extern const cf_int_t CF_SOCK_SHUTDOWN_SEND;
extern const cf_int_t CF_SOCK_SHUTDOWN_RECV;
extern const cf_int_t CF_SOCK_SHUTDOWN_BOTH;


/**
 * @brief definition for socket address.
 * 
 * DON'T use this struct directly.
 * Please use `cf_sockaddr_` functions to fetch or set members.
 */
typedef struct {
#ifdef CF_OS_WIN
    cf_uint16_t sa_family;
#else
    cf_uint8_t  sa_len;
    cf_uint8_t  sa_family;
#endif
    cf_char_t   sa_data[14];
} cf_sockaddr_t;

/**
 * @brief IPv4 address. Internet style.
 * 
 */
typedef union {
    cf_uint32_t ip_u32;
    cf_uint8_t ip_u8x4[4];
} cf_ipv4_addr_t;

/**
 * @brief IPv6 address. Internet style.
 * 
 */
typedef union {
    cf_uint8_t      ip_u8x16[16];
    cf_uint16_t     ip_u16x8[8];
    cf_uint32_t     ip_u32x4[4];
} cf_ipv6_addr_t;

/**
 * @brief Ip Address for ipv4 and ipv6.
 * 
 */
typedef union {
    cf_ipv4_addr_t v4;
    cf_ipv6_addr_t v6;
} cf_ip_addr_t;

/**
 * @brief socket_addr for ipv4. see cf_sockaddr_t
 * 
 * `sin` means socket for internet style(NOT local style).
 */
typedef struct {
#ifdef CF_OS_WIN
    cf_int16_t      sin_family;
#else
    cf_uint8_t      sin_len;
    cf_uint8_t      sin_family;
#endif
    cf_uint16_t     sin_port;
    cf_ipv4_addr_t  sin_addr;
    char		    sin_zero[8];
} cf_sockaddr_ipv4_t;

/**
 * @brief socket_addr for ipv6. see cf_sockaddr_t
 * 
 * `sin` means socket for internet style(NOT local style).
 */
typedef struct {
    cf_uint8_t      sin6_family;   /* AF_INET6 */
    cf_uint16_t     sin6_port;     /* port number */
    cf_uint32_t     sin6_flowinfo; /* IPv6 flow information */
    cf_ipv6_addr_t  sin6_addr;     /* IPv6 address */
    cf_uint32_t     sin6_scope_id; /* Scope ID (new in 2.4) */
} cf_sockaddr_ipv6_t;


void cf_sockaddr_clear(cf_sockaddr_t* addr);
void cf_sockaddr_make_ipv4(cf_sockaddr_t* addr, cf_ipv4_addr_t host, cf_uint16_t port);
void cf_sockaddr_make_ipv6(cf_sockaddr_t* addr, cf_ipv6_addr_t host, cf_uint16_t port);
void cf_ipv4_to_str(cf_ipv4_addr_t host, char* s, cf_size_t len);
void cf_ipv6_to_str(cf_ipv6_addr_t host, char* s, cf_size_t len);
void cf_ipv4_from_str(char* s, cf_size_t len, cf_ipv4_addr_t* host);
void cf_ipv6_from_str(char* s, cf_size_t len, cf_ipv6_addr_t* host);


typedef cf_int_t  cf_socket_t;
typedef cf_uint_t cf_sock_len_t;

cf_uint16_t cf_sock_ntohs(cf_uint16_t n);
cf_uint16_t cf_sock_htons(cf_uint16_t n);
cf_uint32_t cf_sock_ntohl(cf_uint32_t n);
cf_uint32_t cf_sock_htonl(cf_uint32_t n);
cf_char_t* cf_sock_inet_ntoa(cf_ipv4_addr_t in);
cf_errno_t cf_sock_inet_aton(const cf_char_t* s, cf_ipv4_addr_t* addr);
cf_errno_t cf_sock_pton(cf_int_t af, const cf_char_t* src, cf_void_t* dst);
cf_errno_t cf_sock_ntop(cf_int_t af, const cf_void_t* src, cf_char_t* dst, cf_size_t dstsize);

cf_errno_t cf_sock_startup(cf_int_t ver_major, cf_int_t ver_minor);
cf_errno_t cf_sock_cleanup();
cf_errno_t cf_sock_create(cf_socket_t* sock, cf_int_t family, cf_int_t type, cf_int_t protocol);
cf_errno_t cf_sock_shutdown(cf_socket_t sock, cf_int_t how);
cf_errno_t cf_sock_close(cf_socket_t sock);
cf_errno_t cf_sock_bind(cf_socket_t sock, cf_sockaddr_t* sa, cf_sock_len_t addrlen);
cf_errno_t cf_sock_listen(cf_socket_t sock, cf_uint_t backlog);
cf_errno_t cf_sock_accept(cf_socket_t sock, cf_socket_t* new_sock, cf_sockaddr_t* addr, cf_sock_len_t* addrlen);
cf_errno_t cf_sock_connect(cf_socket_t sock, cf_sockaddr_t* sa, cf_int_t namelen);
cf_errno_t cf_sock_send(cf_socket_t sock, cf_void_t* buff, cf_size_t* len, cf_uint_t flags);
cf_errno_t cf_sock_recv(cf_socket_t sock, cf_void_t* buff, cf_size_t* len, cf_uint_t flags);
cf_errno_t cf_sock_sendto(cf_socket_t sock, cf_void_t* buff, cf_size_t* len, cf_uint_t flags, cf_sockaddr_t* sa, cf_sock_len_t addrlen);
cf_errno_t cf_sock_recvfrom(cf_socket_t sock, cf_void_t* buff, cf_size_t* len, cf_uint_t flags, cf_sockaddr_t* from, cf_sock_len_t* fromlen);

cf_errno_t cf_sock_set_nonblock(cf_socket_t sock, cf_bool_t nonblock);



typedef enum {
    CF_SOCKET_FAMILY_UNSPEC,
    CF_SOCKET_FAMILY_LOCAL,
    CF_SOCKET_FAMILY_INET,
    CF_SOCKET_FAMILY_INET6,
} cf_socket_family_t;

typedef enum {
    CF_SOCKET_TYPE_TCP,
    CF_SOCKET_TYPE_UDP,
    CF_SOCKET_TYPE_UNKNOWN,
} cf_socket_type_t;

typedef enum {
    CF_IP_V4,
    CF_IP_V6
} cf_ip_version_t;

typedef struct {
    cf_socket_t         socket_id;
    cf_ip_version_t     ip_version;
    cf_socket_type_t    socket_type;
    cf_ip_addr_t        local_ip;
    cf_uint16_t         local_port;
    cf_ip_addr_t        remote_ip;
    cf_uint16_t         remote_port;
} cf_socket_adv_t;

CF_DECLS_END

#endif /* __CF_SOCKET_H__ */
