/****************************************************
 * Copyright (c) 2018, tkorays <tkorays@hotmail.com>
 * All rights reserved.
 * 
 * @file    zip.h
 * @brief   Zip.
 * @version 1.0
 * @author  tkorays<tkorays@hotmail.com>
 * @date    2018-1-23
 ****************************************************/
#ifndef __CF_UTILS_ZIP_H__
#define __CF_UTILS_ZIP_H__

#include <cf/types.h>

#define CF_ZIP_CREATE       (0)
#define CF_ZIP_CREATEAFTER  (1)
#define CF_ZIP_APPEND       (2)

typedef struct cf_zip_file_item_s {

} cf_zip_file_item_t;

typedef struct cf_zip_file_s cf_zip_file_t;

cf_zip_file_t* cf_zip_open(const cf_char_t* filename, cf_int_t append);
cf_errno_t cf_zip_close(cf_zip_file_t* f);
cf_errno_t cf_zip_open_file_in_zip(cf_zip_file_t* f,
                        const cf_char_t* filename,
                        const cf_char_t* comment);
cf_errno_t cf_zip_close_file_in_zip(cf_zip_file_t* f);
cf_errno_t cf_zip_write_file_in_zip(cf_zip_file_t* f, const cf_void_t* buff, cf_size_t size);



#endif /* __CF_UTILS_ZIP_H__ */
