/****************************************************
 * Copyright (c) 2018, tkorays <tkorays@hotmail.com>
 * All rights reserved.
 * 
 * @file    file.h
 * @brief   File operations.
 * @version 1.0
 * @author  tkorays<tkorays@hotmail.com>
 * @date    2018-1-14
 ****************************************************/
#ifndef __CF_FILE_H__
#define __CF_FILE_H__

#include <cf/types.h>
#include <cf/err.h>
#include <stdio.h>

#define CF_FILE_SEEK_SET    SEEK_SET
#define CF_FILE_SEEK_CUR    SEEK_CUR 
#define CF_FILE_SEEK_END    SEEK_END

typedef enum {
    CF_FILE_TYPE_NOT_DEF = 0,
    CF_FILE_TYPE_REGULAR,
    CF_FILE_TYPE_DIR,
    CF_FILE_TYPE_CHAR,
    CF_FILE_TYPE_BLOCK,
    CF_FILE_TYPE_PIPE,
    CF_FILE_TYPE_LINK,
    CF_FILE_TYPE_SOCK,
    CF_FILE_TYPE_UNKNOWN
} cf_filetype_t;

typedef struct cf_file_s {
    FILE *fp;
} cf_file_t;
typedef struct cf_file_dir_s {
    cf_char_t   __real_impl[16];
} cf_file_dir_t;
typedef struct cf_file_dirent_s {
    cf_char_t   name[CF_MAX_PATH_SIZE];
    cf_char_t   __real_impl[320];
} cf_file_dirent_t;

cf_errno_t  cf_file_open(cf_file_t* f, const cf_char_t* filename, const cf_char_t* mode);
cf_errno_t  cf_file_close(cf_file_t* f);
cf_errno_t  cf_file_write(cf_file_t* f, cf_char_t* buff, cf_size_t size, cf_size_t cnt);
cf_errno_t  cf_file_read(cf_file_t* f, cf_char_t* buff, cf_size_t size, cf_size_t cnt);
cf_bool_t   cf_file_eof(cf_file_t* f);
cf_errno_t  cf_file_getc(cf_file_t* f, cf_char_t* c);
cf_errno_t  cf_file_putc(cf_file_t* f, cf_char_t c);
cf_errno_t  cf_file_gets(cf_file_t* f, cf_char_t* buff, cf_size_t size);
cf_errno_t  cf_file_puts(cf_file_t* f, const cf_char_t* buff);
cf_errno_t  cf_file_printf(cf_file_t* f, const cf_char_t* fmtstr, ...);
cf_errno_t  cf_file_scanf(cf_file_t* f, const cf_char_t* fmtstr, ...);

cf_errno_t  cf_file_opendir(cf_file_dir_t* dir, const cf_char_t* path);
cf_errno_t  cf_file_closedir(cf_file_dir_t* dir);
cf_errno_t cf_file_readdir(cf_file_dir_t* dir, cf_file_dirent_t* dirinfo);

cf_bool_t   cf_file_exist(const cf_char_t* filename);
cf_errno_t  cf_file_rmdir(const cf_char_t* dirname);
cf_errno_t  cf_file_remove(const cf_char_t* filename);
cf_errno_t  cf_file_chdir(const cf_char_t* dirname);
cf_errno_t  cf_file_getcwd(cf_char_t* cwd, cf_size_t size);
cf_errno_t  cf_file_copy(const cf_char_t* src, const cf_char_t* dst);
cf_errno_t  cf_file_rename(const cf_char_t* src, const cf_char_t* dst);
cf_errno_t  cf_file_mkdir(const cf_char_t* path);
cf_errno_t  cf_file_link(const cf_char_t* from_path, const cf_char_t* to_path);
cf_errno_t  cf_file_unlink(const cf_char_t* pathname);

cf_errno_t  cf_file_get_extension(const cf_char_t* path, cf_char_t* buf, cf_size_t size);
cf_errno_t  cf_file_get_basedir(const cf_char_t* path, cf_char_t* buf, cf_size_t size);
cf_errno_t  cf_file_path_join(cf_char_t* buff, cf_size_t size, const cf_char_t* p1, const cf_char_t* p2);

#endif /* __CF_FILE_H__ */
