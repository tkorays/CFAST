#ifndef __CFEXP_TELNET_SERVER_H__
#define __CFEXP_TELNET_SERVER_H__

#include <cf/types.h>
#include <cf/socket.h>
#include <cf/thread.h>
#include <cf/cli.h>

CF_DECLS_BEGIN

#define CF_TELNET_SERV_MAX_USER 5

typedef struct {
    cf_socket_t sock;
    cf_sockaddr_in_t addr;
    cf_bool_t inuse;
} cf_telnet_client_t;

/**
 * Telnet server.
 */
typedef struct {
    cf_socket_t sock;       /** server socket */
    cf_char_t   host[16];   /** server ip address */
    cf_uint16_t port;       /** server port */
    cf_thread_t thr;        /** thread to process accept and read & write */
    cf_telnet_client_t client[CF_TELNET_SERV_MAX_USER];
    cf_cli_t    cli;
} cf_telnet_server_t;


CF_DECLARE(cf_errno_t) cf_telnet_server_create(
    cf_telnet_server_t* serv, 
    const cf_char_t* host, 
    cf_uint16_t port
);

CF_DECLARE(cf_errno_t) cf_telnet_server_destroy(
    cf_telnet_server_t* serv
);



CF_DECLS_END

#endif /* __CFEXP_TELNET_SERVER_H__ */
