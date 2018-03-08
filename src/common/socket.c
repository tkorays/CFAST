#include <cf/socket.h>
#include <cf/err.h>
#include <cf/str.h>

#ifdef CF_OS_WIN
//#include <winsock.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#endif

const cf_int_t CF_SOCK_AF_UNSPEC = AF_UNSPEC;
const cf_int_t CF_SOCK_AF_LOCAL = AF_UNIX;
const cf_int_t CF_SOCK_AF_INET = AF_INET;
const cf_int_t CF_SOCK_AF_INET6 = AF_INET6;

const cf_int_t CF_SOCK_STREAM = SOCK_STREAM;
const cf_int_t CF_SOCK_DGRAM = SOCK_DGRAM;
const cf_int_t CF_SOCK_RAW = SOCK_RAW;

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

cf_char_t*  cf_sock_inet_ntoa(cf_in_addr_t in) {
    static cf_char_t s[18] = {0};
    cf_char_t* addr = (cf_char_t*)&(in.S_addr);
    (cf_void_t)cf_snprintf(s, sizeof(s), "%s.%s.%s.%s", addr[0], addr[1], addr[2], addr[3]);
    return s;
}

cf_errno_t cf_sock_inet_aton(const cf_char_t* s, cf_in_addr_t* addr) {
    if(!s || !addr) return CF_NOK;
    //struct in_addr ia;
    //if(inet_aton(s, &ia) != 0) return CF_NOK;
    //addr->S_addr = (cf_uint32_t)ia.s_addr;
    return inet_pton(CF_SOCK_AF_INET, s, addr) == 0 ? CF_OK : CF_NOK;
    //addr->S_addr = inet_addr(s);
}


cf_errno_t cf_sock_pton(cf_int_t af, const cf_char_t* src, cf_void_t* dst) {
    if (!src || !dst) return CF_EPARAM;
    return inet_pton(af, src, dst) == 0 ? CF_OK : CF_NOK;
}

cf_errno_t cf_sock_ntop(cf_int_t af, const cf_void_t* src, cf_char_t* dst, cf_size_t dstsize) {
    if (!src || !dst) return CF_EPARAM;
    return inet_ntop(af, src, dst, dstsize) == 0 ? CF_OK : CF_NOK;
}

#ifdef CF_OS_WIN
cf_errno_t cf_sock_startup(cf_int_t ver_major, cf_int_t ver_minor) {
    WSADATA wsd;
    return WSAStartup(MAKEWORD(ver_major, ver_minor), &wsd) == 0 ? CF_OK : CF_NOK;
}

cf_errno_t cf_sock_cleanup() {
    return WSACleanup() == 0 ? CF_OK : CF_NOK;
}
#endif

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

cf_errno_t cf_sock_shutdown(cf_socket_t sock, cf_int_t how) {
    return shutdown(sock, how) == 0 ? CF_OK : CF_NOK;
}

cf_errno_t cf_sock_close(cf_socket_t sock) {
    if(!sock) return CF_EPARAM;
#ifdef CF_OS_WIN
    if (0 != closesocket(sock)) return CF_NOK;
#else
    if(0 != close(sock)) return CF_NOK;
#endif
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
    cf_int_t rl = sendto(sock, buff, *len, flags, (struct sockaddr*)sa, addrlen);
    if(rl <= 0) return CF_NOK;                         
    *len = rl;
    return CF_OK;
}

cf_errno_t cf_sock_recvfrom(cf_socket_t sock, cf_void_t* buff, cf_size_t *len, cf_uint_t flags,
                            cf_sockaddr_t* from, cf_sock_len_t* fromlen) {
    cf_int_t rl = recvfrom(sock, buff, *len, flags, (struct sockaddr*)from, fromlen);    
    if(rl <= 0) return CF_NOK;                         
    *len = rl;
    return CF_OK;
}
