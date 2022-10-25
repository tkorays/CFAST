#include "cf/location.h"
#include "cf/assert.h"
#include <stdio.h>

int main(int argc, char const *argv[]) {
    cf_location_define(location);
    printf("location: %s, func: %s, line: %d\n", location.file_name, location.function_name, location.line_number);
    return 0;
}
