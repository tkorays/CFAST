#include <cf/memory.h>
#include <cf/str.h>
#include <stdio.h>

int main() {
    cf_char_t* s = "abced#$%";
    cf_char_t* t = (cf_char_t*)cf_memchr(s, 'c', cf_strlen(s));
    cf_char_t* t2 = cf_strrchr(s, '#');
    printf("%s, %s\n", t, t2);
    return 0;
}
