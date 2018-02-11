#include <cf/socket.h>
#include <cf/err.h>
#include <cf/str.h>
#include <sys/socket.h>
#include <unistd.h>

const cf_int_t CF_SOCK_AF_UNSPEC = AF_UNSPEC;
const cf_int_t CF_SOCK_AF_LOCAL = AF_LOCAL;
const cf_int_t CF_SOCK_AF_INET = AF_INET;
const cf_int_t CF_SOCK_AF_INET6 = AF_INET6;

const cf_int_t CF_SOCK_STREAM = SOCK_STREAM;
const cf_int_t CF_SOCK_DGRAM = SOCK_DGRAM;
const cf_int_t CF_SOCK_RAW = SOCK_RAW;

#ifdef CF_OS_WIN
const cf_int_t CF_SOCK_PROTO_TCP = IPPROTO_TCP;
const cf_int_t CF_SOCK_PROTO_UDP = IPPROTO_UDP;
#else
const cf_int_t CF_SOCK_PROTO_TCP = 6;
const cf_int_t CF_SOCK_PROTO_UDP = 17;
#endif

cf_uint16_t cf_sock_ntohs(uint16_t n) {
    return CF_SWAP16(n);
}

cf_uint16_t cf_sock_htons(uint16_t n) {
    return CF_SWAP16(n);
}

cf_uint32_t cf_sock_ntohl(uint32_t n) {
    return CF_SWAP32(n);
}

cf_uint32_t cf_sock_htonl(cf_uint32_t n) {
    return CF_SWAP32(n);
}

cf_char_t*  cf_sock_inet_ntoa(cf_sockaddr_in_t in) {
    static cf_char_t s[18] = {0};

}

cf_errno_t cf_socket_create(cf_socket_t* sock, cf_int_t family, cf_int_t type, cf_int_t protocol) {
    if(!sock) {
        return CF_EPARAM;
    }
    *sock = socket(family, type, protocol);
    if(!*sock) {
        return CF_NOK;
    }
    return CF_OK;
}

cf_errno_t cf_socket_close(cf_socket_t sock) {
    if(!sock) return CF_EPARAM;
    if(0 != close(sock)) return CF_NOK;
    return CF_OK;
}

cf_errno_t cf_socket_bind(cf_socket_t sock, cf_sockaddr_t* sa, cf_size_t addrlen) {
    if(sock == 0 || !sa) return CF_EPARAM;
    // TODO: sa to sockaddr_in or sockaddr_in6
    if(0 != bind(sock, (struct sockaddr*)sa, addrlen)) return CF_NOK;
    return CF_OK;
}

cf_errno_t cf_socket_listen(cf_socket_t sock, cf_uint_t backlog) {
    if(sock == 0) return CF_EPARAM;
    if(0 != listen(sock, backlog)) return CF_NOK;
    return CF_OK;
}

cf_errno_t cf_socket_accept(cf_socket_t sock, cf_socket_t* new_sock) {
    return CF_OK;
}