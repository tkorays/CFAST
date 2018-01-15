#include "cf_file_if.h"
#include <stdio.h>


cf_errno_t  cf_file_open(cf_file_t** f, const cf_char_t* filename, const cf_char_t* mode) {
    if(!f || !filename || !mode) return CF_ERRNO_INVALID_PARAM;
    
}