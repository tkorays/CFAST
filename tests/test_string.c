#include "cf/string.h"
#include "cf/assert.h"
#include "cf/str.h"


int main(int argc, char* argv[]) {
  cf_string_t str;
  cf_string_clear(&str);
  cf_assert(cf_string_len(&str) == 0);

  cf_assert(CF_TRUE == cf_string_init(&str, "1234", 4));
  cf_assert(cf_string_len(&str) == 4);

  cf_string_deinit(&str);
  cf_assert(cf_string_len(&str) == 0);

  const char* s = "abcdefg";
  cf_string_reset(&str, (cf_char_t*)s, cf_strlen(s));
  cf_assert(s == cf_string_ptr(&str));

  cf_string_init(&str, "abcdefg", 5);
  cf_assert(cf_string_cmp_raw(&str, "abcde", 5) == 0);
  cf_assert(cf_string_cmp_raw(&str, "abc", 3) == 1);
  cf_assert(cf_string_cmp_raw(&str, "abcdefgh", 6) == -1);
  cf_assert(cf_string_cmp_raw(&str, "", 0) == 1);
  return 0;
}
