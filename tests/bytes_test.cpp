#include <gtest/gtest.h>
#include <cf/bytes.h>
#include <iostream>

TEST(cf_bytes, read_write_test) {
    cf_byte_reader_t r;
    cf_byte_writer_t w;
    cf_uint8_t buf[1024];
    cf_byte_writer_init(&w, buf, 1024);
    cf_byte_writer_put_i32(&w, 1234);
    cf_byte_writer_put_double(&w, 1.1111);
    
    cf_byte_reader_init(&r, cf_byte_writer_data(&w), cf_byte_writer_length(&w));
    EXPECT_EQ(cf_byte_reader_remaining(&r), sizeof(int32_t) + sizeof(double));
    cf_int32_t vi = 0;
    double vd = 0.0;
    EXPECT_EQ(cf_byte_reader_get_i32(&r, &vi), CF_TRUE);
    EXPECT_EQ(vi, 1234);
    EXPECT_EQ(cf_byte_reader_get_double(&r, &vd), CF_TRUE);
    EXPECT_EQ(vd, 1.1111);
    EXPECT_EQ(cf_byte_reader_remaining(&r), 0);
}