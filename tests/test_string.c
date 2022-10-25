#include "cf/string.h"
#include "cf/assert.h"


int main(int argc, char* argv[]) {
  cf_string_t str;
  cf_string_clear(&str);
  cf_assert(cf_string_len(&str) == 0);

  cf_assert(CF_TRUE == cf_string_init(&str, 20, CF_NULL_PTR));
  cf_assert(cf_string_len(&str) == 20);

  cf_string_deinit(&str);
  cf_assert(cf_string_len(&str) == 0);

  const char* s = "abcdefg";
  cf_string_reset(&str, (cf_char_t*)s, cf_strlen(s), CF_NULL_PTR);
  cf_assert(s == cf_string_ptr(&str));

  cf_string_init(&str, 5, "abcdefg");
  cf_assert(cf_string_cmp_raw(&str, "abcde", 5) == 0);
  cf_assert(cf_string_cmp_raw(&str, "abc", 3) == 1);
  cf_assert(cf_string_cmp_raw(&str, "abcdefgh", 6) == -1);
  cf_assert(cf_string_cmp_raw(&str, "", 0) == 1);
  return 0;
}
