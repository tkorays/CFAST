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

/**
 * @brief CF_SOCK native sock
 * this is a cross-platform native socket implementation.
 * @{
 */

CF_DECLS_BEGIN

#define CF_SOCK_INVALID     (-1)

extern const cf_int_t       CF_AF_UNSPEC;
extern const cf_int_t       CF_AF_UNIX;
extern const cf_int_t       CF_AF_INET;
extern const cf_int_t       CF_AF_INET6;

extern const cf_int_t       CF_SOCK_STREAM;
extern const cf_int_t       CF_SOCK_DGRAM;
extern const cf_int_t       CF_SOCK_RAW;
extern const cf_int_t       CF_SOCK_RDM;
extern const cf_int_t       CF_SOCK_SEQPACKET;

extern const cf_int_t       CF_SOCK_PROTO_AUTO;
extern const cf_int_t       CF_SOCK_PROTO_TCP;
extern const cf_int_t       CF_SOCK_PROTO_UDP;

/**
 * represent a native socket handle
 */
typedef int cf_sock_t;

/**
 * define the internet ip address for ipv4
 */
typedef struct in_addr cf_in4_addr_t;

/**
 * define the internet ip address for ipv6
 */
typedef struct in6_addr cf_in6_addr_t;

/**
 * @brief socket length
 * 
 */
typedef socklen_t cf_socklen_t;

/**
 * socket address for ipv4
 */
typedef struct {
#if defined(CF_OS_MAC)
    cf_uint8_t      sin_len;        /** length of this structure */
    cf_uint8_t      sin_family;     /** socket family */
#else
    cf_uint16_t     sin_family;     /** sock family */
#endif 
    cf_uint16_t     sin_port;       /** port of this socket */
    cf_in4_addr_t   sin_addr;       /** ipv4 address */
    char            sin_pad[8];     /** zero paddings */
} cf_sockaddr_in4_t;

/**
 * socket address for ipv6
 */
typedef struct {
#if defined(__APPLE__)
    cf_uint8_t      sin_len;        /** length of this structure */
    cf_uint8_t      sin_family;     /** socket family */
#else
    cf_uint16_t     sin_family;     /** socket family */
#endif 
    cf_uint16_t     sin_port;       /** port of this socket */
    cf_uint32_t     sin_flowinfo;   /** flow information */
    cf_in6_addr_t   sin_addr;       /** ipv6 address */
    cf_uint32_t     sin_scope_id;   /** scope id for ipv6 */
} cf_sockaddr_in6_t;

typedef union {
    struct sockaddr     common;
    cf_sockaddr_in4_t   v4;
    cf_sockaddr_in6_t   v6;
} cf_sockaddr_t;

/**
 * @brief convert network byte order to host byte order for 16-bit values
 * 
 * @param n the value to be converted
 * @return cf_uint16_t the converted value
 */
cf_uint16_t cf_sock_ntohs(cf_uint16_t n);

/**
 * @brief convert host byte order to network byte order for 16-bit values
 * 
 * @param n the value to be converted
 * @return cf_uint16_t the converted value
 */
cf_uint16_t cf_sock_htons(cf_uint16_t n);

/**
 * @brief convert network byte order to host byte order for 32-bit values
 * 
 * @param n the value to be converted
 * @return cf_uint32_t the converted value
 */
cf_uint32_t cf_sock_ntohl(cf_uint32_t n);

/**
 * @brief convert host byte order to network byte order for 32-bit values
 * 
 * @param n the value to be converted
 * @return cf_uint32_t the converted value
 */
cf_uint32_t cf_sock_htonl(cf_uint32_t n);
/**
 * @brief convert ipv4 address to string
 * 
 * @param addr      ipv4 address
 * @param buf       buffer to hold the string
 * @param len       length of the buffer
 * @return cf_bool_t    CF_TRUE on success.
 */
cf_bool_t cf_sock_inet_ntoa(cf_in4_addr_t addr, cf_char_t* buf, cf_size_t len);

/**
 * @brief convert string to ipv4 address
 * 
 * @param s         string to be converted
 * @param addr      ipv4 address
 * @return cf_bool_t    CF_TRUE on success.
 */
cf_bool_t cf_sock_inet_aton(const cf_char_t* s, cf_in4_addr_t* addr);
/**
 * @brief convert presentation format address to network format address
 * 
 * @param af    address family
 * @param src   presentation format address
 * @param dst   network format address
 * @return cf_bool_t   CF_TRUE on success.
 */
cf_bool_t cf_sock_pton(cf_int_t af, const cf_char_t* src, cf_void_t* dst);

/**
 * @brief convert network format address to presentation format address
 * 
 * @param af        address family
 * @param src       network format address
 * @param dst       presentation format address
 * @param dstsize   size of the presentation format address buffer
 * @return cf_bool_t   CF_TRUE on success.
 */
cf_bool_t cf_sock_ntop(cf_int_t af, const cf_void_t* src, cf_char_t* dst, cf_size_t dstsize);

/**
 * @brief initialize ipv4 socket address
 * 
 * @param addr      ipv4 socket address
 * @param addr_str  ipv4 address string
 * @param port      port number
 * @return cf_bool_t    CF_TRUE on success.
 */
cf_bool_t cf_sockaddr_in4_init(cf_sockaddr_in4_t* addr, const cf_char_t* addr_str, cf_uint16_t port);

/**
 * @brief prepare for socket.
 * 
 * @return cf_bool_t CF_TRUE for success else failed
 */
cf_bool_t cf_sock_bootstrap();

/**
 * @brief cleanup for socket
 * 
 * @return cf_void_t 
 */
cf_void_t cf_sock_cleanup();

/**
 * @brief get socket error code.
 * 
 * @return cf_uint32_t socket error code
 */
cf_uint32_t cf_sock_get_errno();

/**
 * @brief create a socket with faimily, type and protocol
 * 
 * @param family 
 * @param type 
 * @param protocol 
 * @return cf_sock_t 
 */
cf_sock_t cf_sock_open(int family, int type, int protocol);

/**
 * @brief close a socket
 * 
 * @param sock  native socket handle
 * @return cf_bool_t cF_TRUE for success
 */
cf_bool_t cf_sock_close(cf_sock_t sock);

/**
 * @brief shutdown a socket
 * 
 * @param sock 
 * @param how_sd 
 * @return cf_bool_t 
 */
cf_bool_t cf_sock_shutdown(cf_sock_t sock, int how_sd);

/**
 * @brief bind a address to a socket
 * 
 * @param sock      the native socket handle
 * @param addr      socket address
 * @param addrlen   length of socket address
 * @return cf_bool_t 
 */
cf_bool_t cf_sock_bind(cf_sock_t sock,
                       const cf_sockaddr_t* addr,
                       cf_socklen_t addrlen);

/**
 * @brief listen for incomming connections.
 * 
 * @param sock      the native socket handle
 * @param backlog   maximum pending connections.
 * @return cf_bool_t    CF_TRUE on success.
 */
cf_bool_t cf_sock_listen(cf_sock_t sock, int backlog);

/**
 * @brief accept new sockets.
 * 
 * @param sock      the native socket.
 * @param newsock   new socket
 * @param addr      address for the new socket
 * @param addrlen   length of the new socket address
 * @return cf_bool_t    CF_TRUE on success.
 */
cf_bool_t cf_sock_accept(cf_sock_t sock,
                         cf_sock_t* newsock,
                         cf_sockaddr_t* addr,
                         cf_socklen_t *addrlen);

/**
 * @brief connect to a remote socket.
 * 
 * @param sock      the native socket.
 * @param addr      address to be connected
 * @param addrlen   length of address
 * @return cf_bool_t    CF_TRUE on success.
 */
cf_bool_t cf_sock_connect(cf_sock_t sock,
                          const cf_sockaddr_t* addr,
                          cf_socklen_t addrlen);

/**
 * @brief get opt for socket
 * 
 * @param sock      the native socket handle.
 * @param level     level for opt
 * @param optname   option name
 * @param optval    option value
 * @param optlen    option length
 * @return cf_bool_t    CF_TRUE on success.
 */
cf_bool_t cf_sock_getopt(cf_sock_t sock,
                         cf_uint16_t level,
                         cf_uint16_t optname,
                         cf_void_t* optval,
                         cf_socklen_t* optlen);

/**
 * @brief set option for socket
 * 
 * @param sock      the native socket handle
 * @param level     opition level
 * @param optname   option name
 * @param optval    option value
 * @param optlen    length of option value
 * @return cf_bool_t    CF_TRUE on success.
 */
cf_bool_t cf_sock_setopt(cf_sock_t sock,
                         cf_uint16_t level,
                         cf_uint16_t optname,
                         const cf_void_t* optval,
                         cf_socklen_t optlen);
/**
 * @brief send data to network.
 * 
 * @param sock      the native socket handle
 * @param buf       buffer to be sent
 * @param len       length of data to be sent
 * @param flags     flags for sending
 * @return int      number of bytes sent
 */
int cf_sock_send(cf_sock_t sock, void* buf, cf_size_t len, unsigned flags);

/**
 * @brief send data to specified address.
 * 
 * @param sock      the native socket handle.
 * @param buf       buffer to be sent.
 * @param len       length of data to be sent
 * @param flags     flags for sending
 * @param to        destination address
 * @param tolen     length of the address
 * @return int      number of bytes sent
 */
int cf_sock_sendto(cf_sock_t sock, void* buf, cf_size_t len, unsigned flags, const cf_sockaddr_t* to, cf_socklen_t tolen);

/**
 * @brief receive data
 * 
 * @param sock      the native socket handle.
 * @param buf       buffer to hold the received data.
 * @param len       length of the receive buffer.
 * @param flags     receive flags
 * @return int      number of bytes received
 */
int cf_sock_recv(cf_sock_t sock, void* buf, cf_size_t len, unsigned flags);

/**
 * @brief receive data from specified address.
 * 
 * @param sock      the native socket handle.
 * @param buf       buffer to hold the received data.
 * @param len       length of the receive buffer
 * @param flags     receive flags
 * @param from      address for receiving data
 * @param fromlen   length of address
 * @return int      number of bytes received
 */
int cf_sock_recvfrom(cf_sock_t sock, void* buf, cf_size_t len, unsigned flags, cf_sockaddr_t* from, cf_socklen_t* fromlen);

CF_DECLS_END

/**
 * @}
 */

#endif /* __CF_SOCK_H__ */

