#include <cf/socket.h>
#include <cf/err.h>

#include <sys/socket.h>
#include <unistd.h>

cf_errno_t cf_socket_create(cf_socket_t* sock, cf_sock_af_t family, cf_sock_type_t type, cf_sock_proto_t protocol) {
    int af = AF_UNSPEC;
    int t = SOCK_STREAM;
    int proto = IPPROTO_TCP;
    if(!sock) {
        return CF_EPARAM;
    }
    switch(family) {
        case CF_SOCK_AF_LOCAL:
            af = AF_LOCAL;
            break;
        case CF_SOCK_AF_INET:
            af = AF_INET;
            break;
        case CF_SOCK_AF_INET6:
            af = AF_INET6;
            break;
        default:
            af = AF_UNSPEC;
            break;
    }

    switch(type) {
        case CF_SOCK_STREAM:
            t = SOCK_STREAM;
            break;
        case CF_SOCK_DGRAM:
            t = SOCK_DGRAM;
            break;
        case CF_SOCK_RAW:
            t = SOCK_RAW;
            break;
        default:
            return CF_NOK;
    }

    switch(protocol) {
        case CF_SOCK_PROTO_TCP:
            proto = IPPROTO_TCP;
            break;
        case CF_SOCK_PROTO_UDP:
            proto = IPPROTO_UDP;
            break;
        case CF_SOCK_PROTO_SCTP:
            proto = IPPROTO_SCTP;
            break;
        default:
            return CF_NOK;
    }

    *sock = socket(af, t, proto);
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