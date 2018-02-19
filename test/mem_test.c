#include <cf/memory.h>
#include <cf/str.h>
#include <cf/dbg.h>
#include <stdio.h>

int main() {
    cf_char_t* s = "abced#$^^^^^^^^^^^^^&&&&&&&&&&&^^^^^";
    cf_char_t* t = (cf_char_t*)cf_memchr(s, 'c', cf_strlen(s));
    cf_char_t* t2 = cf_strrchr(s, '#');
    cf_char_t buf[1024];
    printf("%s, %s\n", t, t2);

    cf_dbg_mem2hex(s, 33, buf, 1024);
    printf("%s\n", buf);
    return 0;
}
