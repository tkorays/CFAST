#include <cfast/cf_err_if.h>
#include <stdio.h>

int main() {
    cf_char_t buff[CF_ERR_DESC_MAX_BUFF];
    cf_err_get_info(CF_EPARAM, buff, sizeof(buff));
    printf("%s\n", buff);
    return 0;
}