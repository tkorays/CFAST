#include <cf/socket.h>
#include <cf/err.h>
#include <cf/str.h>
#include <cf/memory.h>

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

void cf_sockaddr_clear(cf_sockaddr_t* addr) {
    cf_membzero(addr, sizeof(cf_sockaddr_t));
}

void cf_sockaddr_make_ipv4(cf_sockaddr_t* addr, cf_ipv4_addr_t host, cf_uint16_t port) {
    cf_sockaddr_clear(addr);
    cf_sockaddr_ipv4_t* addr_ipv4 = CF_TYPE_CAST(cf_sockaddr_ipv4_t*, addr);
    addr_ipv4->sin_family = CF_SOCK_AF_INET;
    addr_ipv4->sin_addr.ip_u32 = cf_sock_htonl(host.ip_u32);
    addr_ipv4->sin_port = cf_sock_htons(port);
}

void cf_sockaddr_make_ipv6(cf_sockaddr_t* addr, cf_ipv6_addr_t host, cf_uint16_t port) {
    cf_sockaddr_clear(addr);
    cf_sockaddr_ipv6_t* addr_ipv6 = CF_TYPE_CAST(cf_sockaddr_ipv6_t*, addr);
    addr_ipv6->sin6_family = CF_SOCK_AF_INET6;
    addr_ipv6->sin6_addr = host;
    addr_ipv6->sin6_port = cf_sock_htons(port);
}

void cf_ipv4_to_str(cf_ipv4_addr_t host, char* s, cf_size_t len) {
    if (!s || len < 18) {
        return;
    }
    cf_snprintf(s, len, "%s.%s.%s.%s", host.ip_u8x4[0], host.ip_u8x4[1], host.ip_u8x4[2], host.ip_u8x4[3]);
}

void cf_ipv6_to_str(cf_ipv6_addr_t host, char* s, cf_size_t len) {

}

void cf_ipv4_from_str(char* s, cf_size_t len, cf_ipv4_addr_t* host) {
    inet_pton(CF_SOCK_AF_INET, s, host);
}

void cf_ipv6_from_str(char* s, cf_size_t len, cf_ipv6_addr_t* host) {

}

cf_uint16_t cf_sock_ntohs(cf_uint16_t n) {
    return CF_SWAP16(n);
}

cf_uint16_t cf_sock_htons(cf_uint16_t n) {
    return CF_SWAP16(n);
}

cf_uint32_t cf_sock_ntohl(cf_uint32_t n) {
    return CF_SWAP32(n);
}

cf_uint32_t cf_sock_htonl(cf_uint32_t n) {
    return CF_SWAP32(n);
}

cf_char_t*  cf_sock_inet_ntoa(cf_ipv4_addr_t in) {
    static cf_char_t s[18] = {0};
    cf_char_t* addr = (cf_char_t*)&(in.ip_u32);
    (cf_void_t)cf_snprintf(s, sizeof(s), "%s.%s.%s.%s", addr[0], addr[1], addr[2], addr[3]);
    return s;
}

cf_errno_t cf_sock_inet_aton(const cf_char_t* s, cf_ipv4_addr_t* addr) {
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


cf_errno_t cf_sock_startup(cf_int_t ver_major, cf_int_t ver_minor) {
#ifdef CF_OS_WIN
    WSADATA wsd;
    return WSAStartup(MAKEWORD(ver_major, ver_minor), &wsd) == 0 ? CF_OK : CF_NOK;
#else
    return CF_OK;
#endif
}

cf_errno_t cf_sock_cleanup() {
#ifdef CF_OS_WIN
    return WSACleanup() == 0 ? CF_OK : CF_NOK;
#else 
    return CF_OK;
#endif
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

cf_errno_t cf_sock_set_nonblock(cf_socket_t sock, cf_bool_t nonblock) {
#ifdef CF_OS_WIN
    unsigned long mode = (nonblock ? 1 : 0);
    return ioctlsocket(sock, FIONBIO,&mode) == 0 ? CF_OK : CF_NOK;
#else
    cf_int_t flags;
	flags = fcntl(sock, F_GETFL, 0);
	if (flags < 0) {
		return CF_NOK;
	}
    if(nonblock) flags |= O_NONBLOCK;
    else flags &= ~O_NONBLOCK;
	if (fcntl(sock, F_SETFL, flags | O_NONBLOCK) < 0) {
		return CF_NOK;
	}
    return CF_OK;
#endif
}
