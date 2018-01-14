/****************************************************
 * Copyright (c) 2018, tkorays <tkorays@hotmail.com>
 * All rights reserved.
 * 
 * @file    cf_file_if.h
 * @brief   File operations.
 * @version 1.0
 * @author  tkorays<tkorays@hotmail.com>
 * @date    2018-1-14
 ****************************************************/
#ifndef __CF_FILE_IF_H__
#define __CF_FILE_IF_H__

#include "cfast/cf_file_if.h"


typedef struct cf_file_s cf_file_t;
typedef struct cf_file_dir_s cf_file_dir_t;
typedef struct cf_file_dirent_s cf_file_dirent_t;


cf_file_t*  cf_file_open(const cf_char_t* filename, const cf_char_t* mode);
cf_errno_t  cf_file_close(cf_file_t* f);
cf_size_t   cf_file_write(cf_file_t* f, cf_char_t* buff, cf_size_t size, cf_size_t cnt);
cf_size_t   cf_file_read(cf_file_t* f, cf_char_t* buff, cf_size_t size, cf_size_t cnt);

cf_errno_t  cf_file_opendir(cf_file_dir_t** dir, const cf_char_t* path);
cf_errno_t  cf_file_closedir(cf_file_dir_t* dir);
cf_file_dirent_t* cf_file_readdir(cf_file_dir_t* dir);

#endif /* __CF_FILE_IF_H__ */
