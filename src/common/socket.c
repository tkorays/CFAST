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

cf_char_t*  cf_sock_inet_ntoa(cf_sockaddr_inet_t in) {
    static cf_char_t s[18] = {0};
    (cf_void_t)cf_snprintf(s, sizeof(s), "%s.%s.%s.%s", in.addr_byte[0], in.addr_byte[1], in.addr_byte[2], in.addr_byte[3]);
    return s;
}

cf_errno_t cf_sock_inet_aton(const cf_char_t* s, cf_sockaddr_inet_t* addr) {
    if(!s || !addr) return CF_NOK;
    return CF_OK;
}

cf_errno_t cf_sock_create(cf_socket_t* sock, cf_int_t family, cf_int_t type, cf_int_t protocol) {
    if(!sock) {
        return CF_EPARAM;
    }
    *sock = socket(family, type, protocol);
    if(!*sock) {
        return CF_NOK;
    }
    return CF_OK;
}

cf_errno_t cf_sock_close(cf_socket_t sock) {
    if(!sock) return CF_EPARAM;
    if(0 != close(sock)) return CF_NOK;
    return CF_OK;
}

cf_errno_t cf_sock_bind(cf_socket_t sock, cf_sockaddr_t* sa, cf_sock_len_t addrlen) {
    if(sock == 0 || !sa) return CF_EPARAM;
    if(0 != bind(sock, (struct sockaddr*)sa, addrlen)) return CF_NOK;
    return CF_OK;
}

cf_errno_t cf_sock_listen(cf_socket_t sock, cf_uint_t backlog) {
    if(sock == 0) return CF_EPARAM;
    if(0 != listen(sock, backlog)) return CF_NOK;
    return CF_OK;
}

cf_errno_t cf_sock_accept(cf_socket_t sock, cf_socket_t* new_sock, cf_sockaddr_t* addr, cf_sock_len_t* addrlen) {
    cf_int_t err = accept(sock, (struct sockaddr*)addr, addrlen);
    if(err < 0) return CF_NOK;
    else {
        *new_sock = err;
        return CF_OK;
    }
}

cf_errno_t cf_sock_connect(cf_socket_t sock, cf_sockaddr_t* sa, cf_int_t namelen) {
    cf_int_t err = connect(sock, (struct sockaddr*)sa, namelen);
    if(err) return CF_NOK;
    else return CF_OK;
}

cf_errno_t cf_sock_send(cf_socket_t sock, cf_void_t* buff, cf_size_t* len, cf_uint_t flags) {
    return cf_sock_sendto(sock, buff, len, flags, CF_NULL_PTR, 0);
}

cf_errno_t cf_sock_recv(cf_socket_t sock, cf_void_t* buff, cf_size_t* len, cf_uint_t flags) {
    return cf_sock_recvfrom(sock, buff, len, flags, CF_NULL_PTR, 0);
}

cf_errno_t cf_sock_sendto(cf_socket_t sock, cf_void_t* buff, cf_size_t *len, cf_uint_t flags,
                          cf_sockaddr_t* sa, cf_sock_len_t addrlen) {
    *len = sendto(sock, buff, *len, flags, (struct sockaddr*)sa, addrlen);
    if(*len > 0) return CF_OK;
    else return CF_NOK;
}

cf_errno_t cf_sock_recvfrom(cf_socket_t sock, cf_void_t* buff, cf_size_t *len, cf_uint_t flags,
                            cf_sockaddr_t* from, cf_sock_len_t* fromlen) {
    *len = recvfrom(sock, buff, *len, flags, (struct sockaddr*)from, fromlen);
    if(*len > 0) return CF_OK;
    else return CF_NOK;
}
