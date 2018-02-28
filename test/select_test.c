#include <cf/select.h>
#include <cf/err.h>
#include <unistd.h>
#include <stdio.h>

int main() {
    cf_fdset_t reads;
    cf_errno_t status;
    cf_timeval_t timeout;
    cf_size_t str_len;
    cf_char_t buf[1024];

    cf_fd_zero(&reads);
    cf_fd_set(0, &reads);

    while(1) {
        timeout.tv_sec = 5;
        timeout.tv_usec = 0;

        status = cf_select(1, &reads, CF_NULL_PTR, CF_NULL_PTR, &timeout);

        if(status != CF_OK) {
            printf("select error\n");
        } else {
            if(cf_fd_isset(0, &reads)) {
                str_len = read(0, buf, 1024);
                buf[str_len] = 0;
                printf("select from reads: %s", buf);
            }
        }
    }
    return 0;
}