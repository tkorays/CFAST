#ifndef __CF_TRANSPORT_H__
#define __CF_TRANSPORT_H__

#include <cf/types.h>

/**
 * @defgroup CF_SOCKET socket
 * socket for network
 * @{
 */

#define CF_SOCK_PORT_UNSPEC 0
#define CF_IP_V4 4
#define CF_IP_V6 6

CF_DECLS_BEGIN

/* ipv4 */
typedef union {
    cf_uint8_t  u8x4[4];
    cf_uint32_t u32;
} cf_ipaddr_v4_t;

/* ipv6 */
typedef union {
    cf_uint8_t  u8x16[16];
    cf_uint16_t u16x8[8];
    cf_uint32_t u32x4[4];
} cf_ipaddr_v6_t;

/**
 * ip address definition
 */
typedef struct {
    union {
        cf_ipaddr_v4_t v4;
        cf_ipaddr_v6_t v6;
    } ipaddr;
    cf_uint16_t     port;

    cf_uint8_t      ipver;
} cf_sockaddr_t;

/**
 * socket for TCP/UDP and IPv4/IPv6.
 */
typedef struct {
    cf_bool_t       is_ipv4;
    cf_bool_t       is_tcp;
    cf_sockaddr_t   addr;
    cf_int_t        raw_socket_id;

    cf_uint64_t     send_err;
    cf_uint64_t     recv_err;
    cf_uint64_t     send_bytes;
    cf_uint64_t     recv_bytes;
} cf_socket_t;

#define cf_socket_is_ipv4(self) ((self)->is_ipv4)
#define cf_socket_is_ipv6(self) (!(self)->is_ipv4)
#define cf_socket_is_tcp(self) ((self)->is_tcp)
#define cf_socket_is_udp(self) (!(self)->is_udp)
#define cf_socket_send_errs(self) ((self)->send_err)
#define cf_socket_recv_errs(self) ((self)->recv_err)
#define cf_socket_send_bytes(self) ((self)->send_bytes)
#define cf_socket_recv_bytes(self) ((self)->recv_bytes)

/**
 * bootstrap the socket
 */
cf_bool_t cf_socket_bootstrap();

/**
 * shutdown the socket
 */
cf_void_t cf_socket_shutdown();

/**
 * initialize the socket object
 */
cf_bool_t cf_socket_init(cf_socket_t* self, cf_bool_t is_ipv4, cf_bool_t is_tcp);

/**
 * destroy the socket object
 */
void cf_socket_deinit(cf_socket_t* self);

/**
 * close socket
 */
cf_bool_t cf_socket_close(cf_socket_t* self);

/**
 *bind the local address, ip:port
 */
cf_bool_t cf_socket_bind(cf_socket_t* self, cf_sockaddr_t* addr);

/**
 * socket listen
 */
cf_bool_t cf_socket_listen(cf_socket_t* self);

/**
 * socket accept
 */
cf_bool_t cf_socket_accept(cf_socket_t* self, cf_socket_t* sock);

/**
 * socket connect
 */
cf_bool_t cf_socket_connect(cf_socket_t* self, cf_sockaddr_t* addr);

/**
 * socket send for TCP 
 */
cf_int_t cf_socket_send(cf_socket_t* self, void* buff, cf_size_t len);

/**
 * socket recv for TCP
 */
cf_int_t cf_socket_recv(cf_socket_t* self, void* buff, cf_size_t len);

/**
 * socket sendto for UDP
 */
cf_int_t cf_socket_sendto(cf_socket_t* self, cf_sockaddr_t* to, void* buff, cf_size_t len);

/**
 * socket recvfrom for UDP
 */
cf_int_t cf_socket_recvfrom(cf_socket_t* self, cf_sockaddr_t* from, void* buff, cf_size_t len);

/**
 * set nonblock socket
 */
cf_bool_t cf_socket_set_nonblock(cf_socket_t* self, cf_bool_t nonblock);

CF_DECLS_END

/** @} */

#endif /* __CF_TRANSPORT_H__ */

