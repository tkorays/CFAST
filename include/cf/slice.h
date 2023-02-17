#ifndef __CF_SLICE_H__
#define __CF_SLICE_H__

#include "cf/types.h"

typedef struct {
    void*       begin;
    cf_size_t   size;
} cf_slice_t;

#endif /* __CF_SLICE_H__ */

