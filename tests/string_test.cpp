#include <gtest/gtest.h>
#include <cf/string.h>

TEST(cf_string_test, construct) {
  cf_string_t str;
  cf_string_clear(&str);
  EXPECT_EQ(cf_string_len(&str), 0);
  cf_string_init(&str, 20);
  EXPECT_EQ(cf_string_len(&str), 20);
}
