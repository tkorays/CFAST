#include "cf/sock.h"

const cf_int_t CF_AF_UNSPEC = AF_UNSPEC;
const cf_int_t CF_AF_UNIX   = AF_UNIX;
const cf_int_t CF_AF_INET   = AF_INET;
const cf_int_t CF_AF_INET6  = AF_INET6;
const cf_int_t CF_AF_IRDA   = AF_IRDA;

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
                       int addrlen) {
    return bind(sock, CF_TYPE_CAST(struct sockaddr*, addr), addrlen) == 0;
}

cf_bool_t cf_sock_listen(cf_sock_t sock, int backlog) {
    return listen(sock, backlog) == 0;   
}

cf_bool_t cf_sock_accept(cf_sock_t sock,
                         cf_sock_t* newsock,
                         cf_sockaddr_t* addr,
                         int *addrlen) {
    *newsock = accept(sock, CF_TYPE_CAST(struct sockaddr*, addr), addrlen);
    if(CF_SOCK_INVALID == *newsock) {
        return CF_FALSE;
    }
    return CF_TRUE;
}

cf_bool_t cf_sock_connect(cf_sock_t sock,
                          const cf_sockaddr_t* addr,
                          int addrlen) {
    return connect(sock, CF_TYPE_CAST(struct sockaddr*, addr), addrlen) == 0;
}

cf_bool_t cf_sock_getopt(cf_sock_t sock,
                         cf_uint16_t level,
                         cf_uint16_t optname,
                         cf_void_t* optval,
                         int* optlen) {
    return getsockopt(sock, level, optname, optval, optlen) == 0;
}

cf_bool_t cf_sock_setopt(cf_sock_t sock,
                         cf_uint16_t level,
                         cf_uint16_t optname,
                         const cf_void_t* optval,
                         int optlen) {
    return setsockopt(sock, level, optname, optval, optlen) == 0;
}

int cf_sock_send(cf_sock_t sock, void* buf, cf_size_t len, unsigned flags) {
    return send(sock, (const char*)buf, (int)len, flags);
}

int cf_sock_sendto(cf_sock_t sock, void* buf, cf_size_t len, unsigned flags, const cf_sockaddr_t* to, int tolen) {
    return sendto(sock, (const char*)buf, (int)len, flags, CF_TYPE_CAST(const struct sockaddr*, to), tolen);
}

int cf_sock_recv(cf_sock_t sock, void* buf, cf_size_t len, unsigned flags) {
    return recv(sock, (char*)buf, (int)len, flags);
}

int cf_sock_recvfrom(cf_sock_t sock, void* buf, cf_size_t len, unsigned flags, cf_sockaddr_t* from, int* fromlen) {
    return recvfrom(sock, (char*)buf, (int)len, flags, CF_TYPE_CAST(struct sockaddr*, from), fromlen);
}
