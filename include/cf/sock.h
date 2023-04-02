#ifndef __CF_SOCK_H__
#define __CF_SOCK_H__

#include "cf/types.h"

#ifdef CF_OS_WIN
//#include <winsock.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#pragma comment(lib, "WS2_32.lib")
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h> /* POSIX's ioctl */
#include <sys/ioctl.h> /* BSD's ioctl, e.g. Mac OSX */
#include <fcntl.h>
#endif

CF_DECLS_BEGIN

/**
 * socket handle
 */
typedef int cf_sock_t;

/**
 * define the internet address
 */
typedef struct in_addr cf_in_addr_t;

/**
 * define the internet address for ipv6
 */
typedef struct in6_addr cf_in6_addr_t;

/**
 * socket address for ipv4
 */
typedef struct {
#if defined(CF_OS_MAC)
    cf_uint8_t      sin_len;
    cf_uint8_t      sim_family;
#else
    cf_uint16_t     sim_family;
#endif 
    cf_uint16_t     sim_port;
    cf_in_addr_t    sin_addr;
    char            sin_pad[8];
} cf_sockaddr_in4_t;

/**
 * socket address for ipv6
 */
typedef struct {
#if defined(CF_OS_MAC)
    cf_uint8_t      sin_len;
    cf_uint8_t      sim_family;
#else
    cf_uint16_t     sim_family;
#endif 
    cf_uint16_t     sim_port;
    cf_uint32_t     sim_flowinfo;
    cf_in6_addr_t   sim_addr;
    cf_uint32_t     sim_scope_id;

} cf_sockaddr_in6_t;

typedef union {
    cf_sockaddr_in4_t    v4;
    cf_sockaddr_in6_t   v6;
} cf_sockaddr_t;


cf_sock_t cf_sock_open(int family, int type, int protocol);
cf_void_t cf_sock_close(cf_sock_t sock);
cf_bool_t cf_sock_bind(cf_sock_t sock,
                       cf_sockaddr_t* addr,
                       int addrlen);

cf_bool_t cf_sock_bind_in(cf_sock_t sock,
                          cf_uint32_t addr,
                          cf_uint16_t port);

cf_bool_t cf_sock_listen(cf_sock_t sock, int backlog);

cf_bool_t cf_sock_accept(cf_sock_t sock,
                         cf_sock_t* newsock,
                         cf_sockaddr_t* addr,
                         int *addrlen);
cf_bool_t cf_sock_connect(cf_sock_t sock,
                          const cf_sockaddr_t* addr,
                          int addrlen);

cf_bool_t cf_sock_getopt(cf_sock_t sock,
                         cf_uint16_t level,
                         cf_uint16_t name,
                         cf_void_t* val,
                         int len);

cf_bool_t cf_sock_setopt(cf_sock_t sock,
                         cf_uint16_t level,
                         cf_uint16_t name,
                         const cf_void_t* val,
                         int len);

cf_errno_t cf_sock_send();
cf_errno_t cf_sock_sendto();
cf_errno_t cf_sock_recv();
cf_errno_t cf_sock_recvfrom();



CF_DECLS_END

#endif /* __CF_SOCK_H__ */

