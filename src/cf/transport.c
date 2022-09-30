#include "cf/transport.h"
#include "cf/memory.h"

#ifdef CF_OS_WIN
//#include <winsock.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h> /* POSIX's ioctl */
#include <sys/ioctl.h> /* BSD's ioctl, e.g. Mac OSX */
#include <fcntl.h>
#endif

const cf_int_t CF_SOCK_PROTO_AUTO = 0;
#ifdef CF_OS_WIN
const cf_int_t CF_SOCK_PROTO_TCP = IPPROTO_TCP;
const cf_int_t CF_SOCK_PROTO_UDP = IPPROTO_UDP;

const cf_int_t CF_SOCK_SHUTDOWN_SEND = SD_SEND;
const cf_int_t CF_SOCK_SHUTDOWN_RECV = SD_RECEIVE;
const cf_int_t CF_SOCK_SHUTDOWN_BOTH = SD_BOTH;
#else
const cf_int_t CF_SOCK_PROTO_TCP = 6;
const cf_int_t CF_SOCK_PROTO_UDP = 17;

const cf_int_t CF_SOCK_SHUTDOWN_SEND = SHUT_WR;
const cf_int_t CF_SOCK_SHUTDOWN_RECV = SHUT_RD;
const cf_int_t CF_SOCK_SHUTDOWN_BOTH = SHUT_RDWR;
#endif

#define MAKE_INTER_SA_FROM_CF_SA_V4(inter, cfaddr) do {     \
    (inter).sin_family = AF_INET;                           \
    (inter).sin_len = sizeof(inter);                        \
    (inter).sin_port = htons((cfaddr).port);                \
    (inter).sin_addr.s_addr = htonl((cfaddr).ipaddr.v4.u32);\
} while(0)

#define MAKE_CF_SA_FROM_INTER_SA(cfaddr, inter) do {\
} while(0)

cf_bool_t cf_socket_bootstrap() {
#ifdef CF_OS_WIN
    WSADATA wsd;
    return WSAStartup(MAKEWORD(2, 0), &wsd) == 0 ? CF_TRUE : CF_FALSE;
#else
    return CF_TRUE;
#endif
}

cf_void_t cf_socket_shutdown() {
#ifdef CF_OS_WIN
    WSACleanup() == 0;
#endif
}

cf_bool_t cf_socket_init(cf_socket_t* self, cf_bool_t is_ipv4, cf_bool_t is_tcp) {
    self->is_ipv4 = is_ipv4;
    self->is_tcp = is_tcp;
    self->raw_socket_id = 0;
    self->send_err = 0;
    self->recv_err = 0;
    self->send_bytes = 0;
    self->recv_bytes = 0;

    self->raw_socket_id = socket(
            is_ipv4 ? AF_INET : AF_INET6,
            is_tcp ? SOCK_STREAM : SOCK_DGRAM,
            CF_SOCK_PROTO_AUTO);
    if (!self->raw_socket_id) {
        return CF_FALSE;
    }
    return CF_TRUE;
}

void cf_socket_deinit(cf_socket_t* self) {
    if (self->raw_socket_id) {
        shutdown(self->raw_socket_id, CF_SOCK_SHUTDOWN_BOTH);
    }
}

cf_bool_t cf_socket_close(cf_socket_t* self) {
    if (self->raw_socket_id) {
#ifdef CF_OS_WIN
    return closesocket(self->raw_socket_id) == 0;
#else
    return close(self->raw_socket_id) == 0;
#endif
    }
    return CF_TRUE;
}

cf_bool_t cf_socket_bind(cf_socket_t* self, cf_sockaddr_t* addr) {
    struct sockaddr sa;
    socklen_t len;

    cf_memcpy_s(&self->addr, sizeof(self->addr), addr, sizeof(cf_sockaddr_t));

    if (!self->raw_socket_id) return CF_FALSE;

    if (self->is_ipv4) {
        MAKE_INTER_SA_FROM_CF_SA_V4(*CF_TYPE_CAST(struct sockaddr_in*, &sa), self->addr);
        len = sizeof(struct sockaddr_in);
    } else {
        /* IPV6 TODO: support ipv6! */
        len = sizeof(struct sockaddr_in6);
    }

    return 0 == bind(self->raw_socket_id, &sa, len);
}

cf_bool_t cf_socket_listen(cf_socket_t* self) {
    if (!self->raw_socket_id) return CF_FALSE;
    return 0 == listen(self->raw_socket_id, 0);
}

cf_bool_t cf_socket_accept(cf_socket_t* self, cf_socket_t* sock) {
    struct sockaddr sa;
    socklen_t len;
    int new_sock = 0;
    int err = 0;
    if (!self->raw_socket_id) return CF_FALSE;
    if (self->is_ipv4) {
        len = sizeof(struct sockaddr_in);
    } else {
        len = sizeof(struct sockaddr_in6);
    }
    err = accept(self->raw_socket_id, (struct sockaddr*)&sa, &len);
    if (err < 0) return CF_FALSE;

    sock->raw_socket_id = err;
    if (self->is_ipv4) {
        sock->is_ipv4 = self->is_ipv4;
        sock->is_tcp = self->is_tcp;
        sock->addr.ipaddr.v4.u32 = ntohl(CF_TYPE_CAST(struct sockaddr_in*, &sa)->sin_addr.s_addr);
        sock->addr.port = ntohs(CF_TYPE_CAST(struct sockaddr_in*, &sa)->sin_port);
    } else {
        /* TODO: support ipv6 */
    }

    return CF_TRUE;
}

cf_bool_t cf_socket_connect(cf_socket_t* self, cf_sockaddr_t* addr) {
    struct sockaddr sa;
    int err;
    int len = self->is_ipv4 ? sizeof(struct sockaddr_in) : sizeof(struct sockaddr_in6);
    if (!self->raw_socket_id) return CF_FALSE;
    err = connect(self->raw_socket_id, &sa, len);
    if (err) return CF_FALSE;
    if (self->is_ipv4) {
        addr->ipver = CF_IP_V4;
        addr->ipaddr.v4.u32 = ntohl(CF_TYPE_CAST(struct sockaddr_in*, &sa)->sin_addr.s_addr);
        addr->port = ntohs(CF_TYPE_CAST(struct sockaddr_in*, &sa)->sin_port);
    } else {
        /* TODO */
    }
    return CF_TRUE;
}

cf_int_t cf_socket_send(cf_socket_t* self, void* buff, cf_size_t len) {
    return cf_socket_sendto(self, CF_NULL_PTR, buff, len);
}

cf_int_t cf_socket_recv(cf_socket_t* self, void* buff, cf_size_t len) {
    return cf_socket_recvfrom(self, CF_NULL_PTR, buff, len);
}

cf_int_t cf_socket_sendto(cf_socket_t* self, cf_sockaddr_t* to, void* buff, cf_size_t len) {
    struct sockaddr sa;
    if (!self->raw_socket_id) return 0;
    MAKE_INTER_SA_FROM_CF_SA_V4(*CF_TYPE_CAST(struct sockaddr_in*, &sa), *to);
    return sendto(self->raw_socket_id, buff, len, 0, &sa, sizeof(struct sockaddr_in));
}

cf_int_t cf_socket_recvfrom(cf_socket_t* self, cf_sockaddr_t* from, void* buff, cf_size_t len) {
    struct sockaddr sa;
    socklen_t sl;
    int ret;
    if (!self->raw_socket_id) return 0;
    ret = recvfrom(self->raw_socket_id, buff, len, 0, &sa, &sl);

    if (self->is_ipv4) {
        from->ipver = self->addr.ipver;
        from->ipaddr.v4.u32 = ntohl(CF_TYPE_CAST(struct sockaddr_in*, &sa)->sin_addr.s_addr);
        from->port = ntohs(CF_TYPE_CAST(struct sockaddr_in*, &sa)->sin_port);
    } else {
        /* TODO */
    }

    return ret;
}

