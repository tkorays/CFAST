#include "cf/sock.h"
#include "cf/str.h"
#include "cf/assert.h"
#include "cf/thread.h"
#include "cf/event.h"
#include "cf/time.h"
#include <stdio.h>

cf_event_t* event = CF_NULL_PTR;

CF_THREAD_DEF_PROC(thread_proc, arg) {
    cf_sock_t sock = CF_SOCK_INVALID;
    cf_sockaddr_t addr;

    cf_time_sleep(10);
    /** wait for server ready */
    cf_event_wait(event, 1000);

    sock = cf_sock_open(CF_AF_INET, CF_SOCK_STREAM, CF_SOCK_PROTO_AUTO);
    cf_assert(cf_sockaddr_in4_init(&addr.v4, "127.0.0.1", 1234) == CF_TRUE);
    cf_assert(cf_sock_connect(sock, &addr, sizeof(addr.v4)) == CF_TRUE);

    cf_assert(cf_sock_send(sock, "hello", 6, 0) == 6);
    cf_sock_close(sock);
    return CF_THREAD_RET(0);
}


int main(int argc, char const *argv[])
{
    cf_sock_t sock = CF_SOCK_INVALID;
    cf_sock_t newsock = CF_SOCK_INVALID;
    cf_sockaddr_t addr;
    int len;
    char buf[1024];

    cf_thread_t th;
    cf_thread_attr_t thattr;

    event = cf_event_new(CF_TRUE, CF_FALSE);

    cf_assert(cf_sock_bootstrap() == CF_TRUE);
    sock = cf_sock_open(CF_AF_INET, CF_SOCK_STREAM, CF_SOCK_PROTO_AUTO);
    cf_assert(sock != CF_SOCK_INVALID);

    cf_assert(cf_sockaddr_in4_init(&addr.v4, "127.0.0.1", 1234) == CF_TRUE);
    cf_assert(1234 == cf_sock_ntohs(addr.v4.sin_port));
    cf_assert(*(cf_uint32_t*)&addr.v4.sin_addr ==  0x0100007F);

    cf_assert(cf_sock_bind(sock, &addr, sizeof(addr.v4)) == CF_TRUE);
    cf_assert(cf_sock_listen(sock, 5) == CF_TRUE);

    /** create a new thread to act as a client */
    cf_thread_attr_init(&thattr);
    cf_thread_create(&th, &thattr, thread_proc, CF_NULL_PTR);

    /** notify client to send data */
    cf_event_set(event);
    cf_sock_accept(sock, &newsock, CF_NULL_PTR, CF_NULL_PTR);
    len = cf_sock_recv(newsock, buf, sizeof(buf), 0);
    cf_assert(len > 0);
    cf_assert(cf_strcmp(buf, "hello") == 0);
    cf_assert(cf_sock_close(newsock) == CF_TRUE);

    cf_assert(cf_sock_close(sock) == CF_TRUE);
    cf_sock_cleanup();

    cf_event_delete(event);
    return 0;
}
