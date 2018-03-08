#include <cfexp/telnet_server.h>
#include <stdio.h>
#include <unistd.h>

int main() {
    cf_telnet_server_t serv;
    cf_errno_t status;
    status = cf_telnet_server_create(&serv, "0.0.0.0", 9999);
    printf("create status: %d\n", status);
    getchar();
    return 0;
}