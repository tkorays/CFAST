#include <cfexp/telnet_server.h>
#include <stdio.h>

int main() {
    cf_telnet_server_t serv;
    cf_errno_t status;
    status = cf_telnet_server_create(&serv, "0.0.0.0", 9876);
    printf("create status: %d\n", status);
    getchar();
    return 0;
}