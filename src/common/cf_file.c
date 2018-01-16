#include "cfast/cf_file_if.h"
#include "cfast/cf_err_if.h"
#include "cfast/cf_str_if.h"
#include <stdio.h>
#include <stdarg.h>

#ifdef CF_OS_WIN
    #include <io.h>
#else 
    #include<sys/types.h>
    #include<dirent.h>
#endif

typedef struct cf_file_s {
    FILE *fp;
} cf_file_t;

typedef struct __dir_s {
#ifdef CF_OS_WIN
    struct _finddata_t fileinfo;
    long handle;
#else
    DIR* dir;
#endif
} __dir_t;

typedef struct __dirent_s {
#ifdef CF_OS_WIN
    struct _finddata_t fileinfo;
#else
    struct dirent* pdir;
#endif
} __dirent_t;

cf_errno_t  cf_file_open(cf_file_t* f, const cf_char_t* filename, const cf_char_t* mode) {
    if(!f || !filename || !mode) return CF_EPARAM;
    f->fp = fopen(filename, mode);
    if(!f->fp) {
        return CF_EFOPEN;
    }
    return CF_OK;
}

cf_errno_t  cf_file_close(cf_file_t* f) {
    if(!f) {
        return CF_EPARAM;
    }
    if(fclose(f->fp) != 0) {
        return CF_EFCLOSE;
    }
    return CF_OK;
}

cf_errno_t  cf_file_write(cf_file_t* f, cf_char_t* buff, cf_size_t size, cf_size_t cnt) {
    cf_size_t wsize = 0;
    if(!f || !buff || size == 0 || cnt == 0) return CF_EPARAM;
    if(!f->fp) return CF_EFILE_HANDLE;
    wsize = fwrite(buff, size, cnt, f->fp);
    if(wsize == 0) return CF_EFWRITE;
    return CF_OK;
}

cf_errno_t  cf_file_read(cf_file_t* f, cf_char_t* buff, cf_size_t size, cf_size_t cnt) {
    cf_size_t rsize = 0;
    if(!f || !buff || size == 0 || cnt == 0) return CF_EPARAM;
    if(!f->fp) return CF_EFILE_HANDLE;
    rsize = fread(buff, size, cnt, f->fp);
    if(rsize == 0) {
        return CF_EFREAD;
    }
    return CF_OK;
}

cf_bool_t cf_file_eof(cf_file_t* f) {
    if(!f || !f->fp) return CF_TRUE;
    return feof(f->fp) ? CF_TRUE : CF_FALSE;
}

cf_errno_t  cf_file_getc(cf_file_t* f, cf_char_t* c) {
    if(!f || !c) return CF_EPARAM;
    if(!f->fp) return CF_EFILE_HANDLE;
    *c = (cf_char_t)fgetc(f->fp);
    return CF_OK;
}

cf_errno_t  cf_file_putc(cf_file_t* f, cf_char_t c) {
    if(!f) return CF_EPARAM;
    if(!f->fp) return CF_EFILE_HANDLE;
    if(c != (cf_char_t)fputc(c, f->fp)) {
        return CF_EFWRITE;
    }
    return CF_OK;
}

cf_errno_t  cf_file_gets(cf_file_t* f, cf_char_t* buff, cf_size_t size) {
    if(!f || !buff) return CF_EPARAM;
    if(!f->fp) return CF_EFILE_HANDLE;
    if(!fgets(buff, size, f->fp)) {
        return CF_EFREAD;
    }
    return CF_OK;
}

cf_errno_t  cf_file_puts(cf_file_t* f, const cf_char_t* buff) {
    if(!f || !buff) return CF_EPARAM;
    if(!f->fp) return CF_EFILE_HANDLE;
    if(fputs(buff, f->fp) <= 0) {
        return CF_EFWRITE;
    }
    return CF_OK;
}

cf_errno_t  cf_file_printf(cf_file_t* f, const cf_char_t* fmtstr, ...) {
    va_list args;
    cf_int_t ret = 0;
    if(!f || !fmtstr) return CF_EPARAM;
    if(!f->fp) return CF_EFILE_HANDLE;
    va_start(args, fmtstr);
    ret = vfprintf(f->fp, fmtstr, args);
    va_end(args);
    return (ret == -1) ? CF_EFWRITE : CF_OK;
}

cf_errno_t  cf_file_scanf(cf_file_t* f, const cf_char_t* fmtstr, ...) {
    va_list args;
    cf_int_t ret = 0;
    if(!f || !fmtstr) return CF_EPARAM;
    if(!f->fp) return CF_EFILE_HANDLE;
    va_start(args, fmtstr);
    ret = vfscanf(f->fp, fmtstr, args);
    va_end(args);
    return (ret == -1) ? CF_EFWRITE : CF_OK;
}

cf_errno_t  cf_file_opendir(cf_file_dir_t* dir, const cf_char_t* path) {
    __dir_t* _dir;
#ifdef CF_OS_WIN
    cf_char_t path_pattern[CF_MAX_PATH_SIZE] = {0};
#endif
    if(!dir || !path) return CF_EPARAM;
    _dir = (__dir_t*)dir->__real_impl;
#ifdef CF_OS_WIN
    if(CF_OK != cf_file_path_join(path_pattern, sizeof(path_pattern), path, "*")) {
        return CF_NOK;
    }
    _dir->handle = _findfirst(path_pattern, &_dir->fileinfo);
    if(_dir->handle == -1L) return CF_NOK;
#else
    _dir->dir = opendir(path);
    if(!_dir->dir) return CF_NOK;
#endif
    return CF_OK;
}
cf_errno_t  cf_file_closedir(cf_file_dir_t* dir) {
    __dir_t* _dir;
    if(!dir) return CF_EPARAM;
    _dir = (__dir_t*)dir->__real_impl;
#ifdef CF_OS_WIN
    if(!_dir->handle) return CF_ENULLPTR;
    if(!_findclose(_dir->handle)) return CF_NOK;
#else
    if(!_dir->dir) return CF_ENULLPTR;
    if(closedir(_dir->dir) != 0) return CF_NOK;
#endif
    return CF_OK;
}

cf_errno_t cf_file_readdir(cf_file_dir_t* dir, cf_file_dirent_t* dirinfo) {
    __dir_t* _dir;
    __dirent_t* _dirent;
    if(!dir || !dirinfo) return CF_EPARAM;
    _dir = (__dir_t*)dir->__real_impl;
    _dirent = (__dirent_t*)dirinfo->__real_impl;
#ifdef CF_OS_WIN
    if(!_dir->handle) return CF_ENULLPTR;
    if(_findnext(_dir->handle, &_dirent->fileifo) == -1L) return CF_EEOF;
    cf_strcpy_s(dirinfo->name, sizeof(dirinfo->name), _dirent->fileifo.name);
#else
    _dirent->pdir = readdir(_dir->dir);
    if(!_dirent->pdir) return CF_EEOF;
    cf_strcpy_s(dirinfo->name, sizeof(dirinfo->name), _dirent->pdir->d_name);
#endif
    return CF_OK;
}
