#include "cfx/ini.h"
#include "cf/assert.h"
#include <stdio.h>

char* ini_str[] = {
    "; abc",
    "a = 1234",
    "b=1234",
    "[section]",
    "asd = 1234",
    "asdf = 1234"
    "",
    "[sss]",
    "asdasdf = 1234"
};

int main(int argc, char const *argv[])
{
    char buffer[1024];
    int i;
    cfx_ini_t* ini = cfx_ini_new();
    for (i = 0; i < CF_ARRAY_SIZE(ini_str); i++) {
        if (!cfx_ini_input(ini, ini_str[i])) {
            printf("parse error!\n");
        }
    }

    cf_assert(cfx_ini_get(ini, "section", "asdf", buffer, sizeof(buffer)));
    cfx_ini_delete(ini);
    return 0;
}