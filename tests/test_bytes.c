#include "cf/bytes.h"
#include "cf/assert.h"
#include <stdio.h>

int main(int argc, char const *argv[]) {
    cf_byte_reader_t r;
    cf_byte_writer_t w;
    cf_uint8_t buf[1024];
    cf_byte_writer_init(&w, buf, 1024);
    cf_byte_writer_put_i32(&w, 1234);
    cf_byte_writer_put_double(&w, 1.1111);
    
    cf_byte_reader_init(&r, cf_byte_writer_data(&w), cf_byte_writer_length(&w));
    cf_assert(cf_byte_reader_remaining(&r) == sizeof(cf_int32_t) + sizeof(double));
    cf_int32_t vi = 0;
    double vd = 0.0;
    cf_assert(cf_byte_reader_get_i32(&r, &vi) == CF_TRUE);
    cf_assert(vi == 1234);
    cf_assert(cf_byte_reader_get_double(&r, &vd) == CF_TRUE);
    cf_assert(vd == 1.1111);
    cf_assert(cf_byte_reader_remaining(&r) == 0);
    return 0;
}
