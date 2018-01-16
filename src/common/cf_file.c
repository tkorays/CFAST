#include "cfast/cf_file_if.h"
#include "cfast/cf_err_if.h"
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

typedef struct cf_file_dir_s {
#ifdef CF_OS_WIN
    struct _finddata_t *fileinfo;
    long handle;
#else
    DIR* dir;
#endif
} cf_file_dir_t;

typedef struct cf_file_dirent_s {
#ifdef CF_OS_WIN
    struct _finddata_t *fileinfo;
#else
    dirent* pdir;
#endif
} cf_file_dirent_t;

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
    if(!f || !buf) return CF_EPARAM;
    if(!f->fp) return CF_EFILE_HANDLE;
    if(!fgets(buff, size, f->fp)) {
        return CF_EFREAD;
    }
    return CF_OK;
}

cf_errno_t  cf_file_puts(cf_file_t* f, const cf_char_t* buff) {
    if(!f || !buf) return CF_EPARAM;
    if(!f->fp) return CF_EFILE_HANDLE;
    if(fputs(buff, f->fp) <= 0) {
        return CF_EFWRITE;
    }
    return CF_OK;
}

cf_errno_t  cf_file_printf(cf_file_t* f, const cf_char_t* fmtstr, ...) {
    va_list args;
    cf_int_t ret = 0;
    if(!f || !buf) return CF_EPARAM;
    if(!f->fp) return CF_EFILE_HANDLE;
    va_start(args, fmtstr);
    ret = vfprintf(f->fp, fmtstr, args);
    va_end(args);
    return (ret == -1) ? CF_EFWRITE : CF_OK;
}

cf_errno_t  cf_file_scanf(cf_file_t* f, const cf_char_t* fmtstr, ...) {
    va_list args;
    cf_int_t ret = 0;
    if(!f || !buf) return CF_EPARAM;
    if(!f->fp) return CF_EFILE_HANDLE;
    va_start(args, fmtstr);
    ret = vfscanf(f->fp, fmtstr, args);
    va_end(args);
    return (ret == -1) ? CF_EFWRITE : CF_OK;
}

cf_errno_t  cf_file_opendir(cf_file_dir_t* dir, const cf_char_t* path) {
    if(!dir || !path) return CF_EPARAM;
#ifdef CF_OS_WIN
    dir->handle = _findfirst(path, dir->dir);
    if(dir->handle == -1L) return CF_NOK;
#else
    dir->dir = opendir(path);
    if(!dir->dir) return CF_ENOK;
#endif
    return CF_OK;
}
cf_errno_t  cf_file_closedir(cf_file_dir_t* dir) {
    if(!dir) return CF_EPARAM;
#ifdef CF_OS_WIN
    if(!dir->handle) return CF_ENULLPTR;
    if(!_findclose(dir->handle)) return CF_NOK;
#else
    if(!dir->dir) return CF_ENULLPTR;
    if(closedir(dir->dir) != 0) return CF_NOK;
#endif
    return CF_OK;
}

cf_errno_t cf_file_readdir(cf_file_dir_t* dir, cf_file_dirent_t* dirinfo) {
#ifdef CF_OS_WIN
    cf_int_t ret = 0; 
    if(!dir) return CF_EPARAM;
    if(!dir->handle) return CF_ENULLPTR;
    ret = _findnext(dir->handle, dirinfo);
    if(ret == 0) return CF_EEOF;
    else if(ret == -1L) return CF_NOK;
#else
    if(!dir) return CF_EPARAM;
    if(!dir->pdir) return CF_ENULLPTR;
    dirinfo->pdir = readdir(dir->dir);
    if(!dirinfo->pdir) return CF_EEOF;
#endif
    return CF_OK;
}

