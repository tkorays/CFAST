#include "cf/str.h"
#include "cf/assert.h"
#include <stdio.h>

int main(int argc, char *argv[])
{
    cf_char_t s[] = "  12345\t ";   
    cf_char_t s1[] = "a1234bcdefg";

    cf_char_t buf[100];

    cf_assert(cf_str_strip(s));
    cf_assert(cf_strcmp("12345", s) == 0);

    cf_assert(cf_str_capitalize(s1));
    cf_assert(cf_strcmp(s1, "A1234bcdefg") == 0);

    cf_assert(cf_str_switch_case(s1));
    cf_assert(cf_strcmp(s1, "a1234BCDEFG") == 0);

    cf_assert(cf_str_to_upper(s1));
    cf_assert(cf_strcmp(s1, "A1234BCDEFG") == 0);

    cf_assert(cf_str_to_lower(s1));
    cf_assert(cf_strcmp(s1, "a1234bcdefg") == 0);

    cf_assert(cf_str_count_for("11223311", '1') == 4);

    cf_str_center(buf, 100, "abcde", '*', 10);
    cf_assert(cf_strcmp(buf, "**abcde***") == 0);
    return 0;
}
