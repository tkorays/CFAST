#include "cf/assert.h"
#include "cf/fifobuf.h"
#include "cf/str.h"
#include "cf/memory.h"
#include <stdio.h>


int main(int argc, char const *argv[])
{
    cf_char_t buf[100];
    cf_char_t r[10];
    cf_size_t size;
    cf_fifobuf_t* fifo = cf_fifobuf_new(buf, sizeof(buf));

    cf_assert(cf_fifobuf_write(fifo, "12345", 1));
    cf_assert(cf_fifobuf_write(fifo, "12345", 2));
    cf_assert(cf_fifobuf_write(fifo, "12345", 3));
    cf_assert(cf_fifobuf_write(fifo, "12345", 4));
    cf_assert(cf_fifobuf_write(fifo, "12345", 5));

    size = sizeof(r);
    cf_membzero(r, size);
    cf_assert(cf_fifobuf_read(fifo, r, &size));
    cf_assert(size == 1);
    cf_assert(cf_strcmp(r, "1") == 0);

    size = sizeof(r);
    cf_membzero(r, size);
    cf_assert(cf_fifobuf_read(fifo, r, &size));
    cf_assert(size == 2);
    cf_assert(cf_strcmp(r, "12") == 0);

    size = sizeof(r);
    cf_membzero(r, size);
    cf_assert(cf_fifobuf_read(fifo, r, &size));
    cf_assert(size == 3);
    cf_assert(cf_strcmp(r, "123") == 0);

    size = sizeof(r);
    cf_membzero(r, size);
    cf_assert(cf_fifobuf_read(fifo, r, &size));
    cf_assert(size == 4);
    cf_assert(cf_strcmp(r, "1234") == 0);

    size = sizeof(r);
    cf_membzero(r, size);
    cf_assert(cf_fifobuf_read(fifo, r, &size));
    cf_assert(size == 5);
    cf_assert(cf_strcmp(r, "12345") == 0);

    size = sizeof(r);
    cf_assert(cf_fifobuf_read(fifo, r, &size) == CF_FALSE);

    cf_fifobuf_delete(fifo);
    return 0;
}
