#include "cfx/ascii_art.h"
#include "cf/assert.h"
#include <stdio.h>


int main() {
    cf_char_t buf[1024];
    cf_assert(cfx_art_print("Hello world!", buf, 1024, CFX_ART_FONT_DEF) == CF_TRUE);
    printf("%s\n", buf);
    cf_assert(cfx_art_print("Hello world!", buf, 1024, CFX_ART_FONT_SLANT) == CF_TRUE);
    printf("%s\n", buf);
    return 0;
}
