#include <gtest/gtest.h>
#include <cf/location.h>
#include <iostream>

TEST(cf_location_test, test) {
    cf_location_define(location);
    std::cout << location.file_name << "," << location.function_name
        << "," << location.line_number;
}