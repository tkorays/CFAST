#include <gtest/gtest.h>
#include <cf/string.h>

TEST(cf_string_test, construct) {
  cf_string_t str;
  cf_string_clear(&str);
  EXPECT_EQ(cf_string_len(&str), 0);

  EXPECT_EQ(CF_TRUE, cf_string_init(&str, 20, CF_NULL_PTR));
  EXPECT_EQ(cf_string_len(&str), 20);

  cf_string_deinit(&str);
  EXPECT_EQ(cf_string_len(&str), 0);

  const char* s = "abcdefg";
  cf_string_reset(&str, (cf_char_t*)s, cf_strlen(s), CF_NULL_PTR);
  EXPECT_EQ(s, cf_string_ptr(&str));

  cf_string_init(&str, 5, "abcdefg");
  EXPECT_EQ(cf_string_cmp_raw(&str, "abcde", 5), 0);
  EXPECT_EQ(cf_string_cmp_raw(&str, "abc", 3), 1);
  EXPECT_EQ(cf_string_cmp_raw(&str, "abcdefgh", 6), -1);
  EXPECT_EQ(cf_string_cmp_raw(&str, "", 0), 1);
}
