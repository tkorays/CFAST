#include "cf/sock.h"
#include "cf/str.h"
#include "cf/memory.h"

const cf_int_t CF_AF_UNSPEC = AF_UNSPEC;
const cf_int_t CF_AF_UNIX   = AF_UNIX;
const cf_int_t CF_AF_INET   = AF_INET;
const cf_int_t CF_AF_INET6  = AF_INET6;

const cf_int_t CF_SOCK_STREAM       = SOCK_STREAM;
const cf_int_t CF_SOCK_DGRAM        = SOCK_DGRAM;
const cf_int_t CF_SOCK_RAW          = SOCK_RAW;
const cf_int_t CF_SOCK_RDM          = SOCK_RDM;
const cf_int_t CF_SOCK_SEQPACKET    = SOCK_SEQPACKET;


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

cf_uint16_t cf_sock_ntohs(cf_uint16_t n) {
    return ntohs(n);
}

cf_uint16_t cf_sock_htons(cf_uint16_t n) {
    return htons(n);
}

cf_uint32_t cf_sock_ntohl(cf_uint32_t n) {
    return ntohl(n);
}

cf_uint32_t cf_sock_htonl(cf_uint32_t n) {
    return htonl(n);
}

cf_bool_t cf_sock_inet_ntoa(cf_in4_addr_t addr, cf_char_t* buf, cf_size_t len) {
    cf_char_t* p = (cf_char_t*)&(addr);
    return cf_snprintf(buf, len, "%s.%s.%s.%s", p[0], p[1], p[2], p[3]) > 0;
}

cf_bool_t cf_sock_inet_aton(const cf_char_t* s, cf_in4_addr_t* addr) {
    return inet_pton(CF_AF_INET, s, addr) == 0;
}

cf_bool_t cf_sock_pton(cf_int_t af, const cf_char_t* src, cf_void_t* dst) {
    return inet_pton(af, src, dst) == 0;
}

cf_bool_t cf_sock_ntop(cf_int_t af, const cf_void_t* src, cf_char_t* dst, cf_size_t dstsize) {
    return inet_ntop(af, src, dst, dstsize) == 0;
}

cf_bool_t cf_sockaddr_in4_init(cf_sockaddr_in4_t* addr, const cf_char_t* addr_str, cf_uint16_t port) {
#if defined(__APPLE__)
    addr->sin_len = sizeof(cf_sockaddr_in4_init);
#endif
    addr->sin_family    = CF_AF_INET;
    addr->sin_port      = cf_hton_u16(port);
    cf_sock_pton(CF_AF_INET, addr_str, &addr->sin_addr);
    cf_membzero(addr->sin_pad, sizeof(addr->sin_pad));
    return CF_TRUE;
}

cf_bool_t cf_sock_bootstrap() {
#ifdef CF_OS_WIN
    WSADATA wsd;
    return WSAStartup(MAKEWORD(2, 0), &wsd) == 0 ? CF_TRUE : CF_FALSE;
#else
    return CF_TRUE;
#endif
}

cf_void_t cf_sock_cleanup() {
#ifdef CF_OS_WIN
    WSACleanup();
#endif
}

cf_uint32_t cf_sock_get_errno() {
#ifdef CF_OS_WIN
    return WSAGetLastError();
#else
    return -1;
#endif
}

cf_sock_t cf_sock_open(int family, int type, int protocol) {
    return socket(family, type, protocol);
}

cf_bool_t cf_sock_close(cf_sock_t sock) {
    if (sock != CF_SOCK_INVALID) {
#ifdef CF_OS_WIN
        return closesocket(sock) == 0;
#else
        return close(sock) == 0;
#endif
    }
    return CF_TRUE;
}

cf_bool_t cf_sock_shutdown(cf_sock_t sock, int how_sd) {
    return shutdown(sock, how_sd) == 0;
}

cf_bool_t cf_sock_bind(cf_sock_t sock,
                       const cf_sockaddr_t* addr,
                       cf_socklen_t addrlen) {
    return bind(sock, CF_TYPE_CAST(struct sockaddr*, addr), addrlen) == 0;
}

cf_bool_t cf_sock_listen(cf_sock_t sock, int backlog) {
    return listen(sock, backlog) == 0;   
}

cf_bool_t cf_sock_accept(cf_sock_t sock,
                         cf_sock_t* newsock,
                         cf_sockaddr_t* addr,
                         cf_socklen_t *addrlen) {
    *newsock = accept(sock, CF_TYPE_CAST(struct sockaddr*, addr), addrlen);
    if(CF_SOCK_INVALID == *newsock) {
        return CF_FALSE;
    }
    return CF_TRUE;
}

cf_bool_t cf_sock_connect(cf_sock_t sock,
                          const cf_sockaddr_t* addr,
                          cf_socklen_t addrlen) {
    return connect(sock, CF_TYPE_CAST(struct sockaddr*, addr), addrlen) == 0;
}

cf_bool_t cf_sock_getopt(cf_sock_t sock,
                         cf_uint16_t level,
                         cf_uint16_t optname,
                         cf_void_t* optval,
                         cf_socklen_t* optlen) {
    return getsockopt(sock, level, optname, optval, optlen) == 0;
}

cf_bool_t cf_sock_setopt(cf_sock_t sock,
                         cf_uint16_t level,
                         cf_uint16_t optname,
                         const cf_void_t* optval,
                         cf_socklen_t optlen) {
    return setsockopt(sock, level, optname, optval, optlen) == 0;
}

int cf_sock_send(cf_sock_t sock, void* buf, cf_size_t len, unsigned flags) {
    return send(sock, (const char*)buf, (int)len, flags);
}

int cf_sock_sendto(cf_sock_t sock, void* buf, cf_size_t len, unsigned flags, const cf_sockaddr_t* to, cf_socklen_t tolen) {
    return sendto(sock, (const char*)buf, (int)len, flags, CF_TYPE_CAST(const struct sockaddr*, to), tolen);
}

int cf_sock_recv(cf_sock_t sock, void* buf, cf_size_t len, unsigned flags) {
    return recv(sock, (char*)buf, (int)len, flags);
}

int cf_sock_recvfrom(cf_sock_t sock, void* buf, cf_size_t len, unsigned flags, cf_sockaddr_t* from, cf_socklen_t* fromlen) {
    return recvfrom(sock, (char*)buf, (int)len, flags, CF_TYPE_CAST(struct sockaddr*, from), fromlen);
}
