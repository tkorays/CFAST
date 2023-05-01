#include "cfx/ini.h"
#include "cf/assert.h"
#include "cf/memory.h"
#include <stdio.h>

char* ini_str[] = {
    "; abc",
    "a = 1234",
    "b=5678",
    "[section]",
    "asd = 1234",
    "asdf = 1234",
    "",
    "[sss]",
    "asdasdf = 1234"
};

int main(int argc, char const *argv[])
{
    char buffer[1024];
    int i;
    cfx_ini_t* ini;

    cf_memchk_init();

    ini = cfx_ini_new();
    for (i = 0; i < CF_ARRAY_SIZE(ini_str); i++) {
        if (!cfx_ini_input(ini, ini_str[i])) {
            printf("parse error!\n");
        }
    }

    cf_assert(cfx_ini_get(ini, "section", "asdf", buffer, sizeof(buffer)));
    cf_assert(cfx_ini_set(ini, "section", "hello", "world"));
    cf_assert(cfx_ini_set(ini, "biu", "hello", "world"));
    cf_assert(cfx_ini_save(ini, CF_NULL_PTR) == CF_FALSE);
    cf_assert(cfx_ini_save(ini, "test.ini"));
    cfx_ini_delete(ini);
    
    cf_assert(cf_memchk_deinit_and_summary());
    return 0;
}
