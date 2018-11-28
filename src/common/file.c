#include <cf/file.h>
#include <cf/err.h>
#include <cf/str.h>
#include <stdio.h>
#include <stdarg.h>

#ifdef CF_OS_WIN
    #include <windows.h>
    #include <io.h>
    #include <direct.h>
#else
    #include <unistd.h>
    #include<sys/types.h>
    #include <sys/stat.h>
    #include<dirent.h>
    #include<fcntl.h>
#endif

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
#ifdef CF_OS_WIN
    return fopen_s(f, filename, mode) == 0 ? CF_OK : CF_NOK;
#else
    *f = fopen(filename, mode);
    if(!*f) {
        return CF_EFOPEN;
    }
    return CF_OK;
#endif
}

cf_errno_t  cf_file_close(cf_file_t f) {
    if(!f) {
        return CF_EPARAM;
    }
    if(fclose(f) != 0) {
        return CF_EFCLOSE;
    }
    return CF_OK;
}

cf_errno_t  cf_file_write(cf_file_t f, cf_char_t* buff, cf_size_t size, cf_size_t cnt) {
    cf_size_t wsize = 0;
    if(!f || !buff || size == 0 || cnt == 0) return CF_EPARAM;
    wsize = fwrite(buff, size, cnt, f);
    if(wsize == 0) return CF_EFWRITE;
    return CF_OK;
}

cf_errno_t  cf_file_read(cf_file_t f, cf_char_t* buff, cf_size_t size, cf_size_t cnt) {
    cf_size_t rsize = 0;
    if(!f || !buff || size == 0 || cnt == 0) return CF_EPARAM;
    rsize = fread(buff, size, cnt, f);
    if(rsize == 0) {
        return CF_EFREAD;
    }
    return CF_OK;
}

cf_bool_t cf_file_eof(cf_file_t f) {
    if(!f) return CF_TRUE;
    return feof(f) ? CF_TRUE : CF_FALSE;
}

cf_errno_t  cf_file_getc(cf_file_t f, cf_char_t* c) {
    if(!f || !c) return CF_EPARAM;
    *c = (cf_char_t)fgetc(f);
    return CF_OK;
}

cf_errno_t  cf_file_putc(cf_file_t f, cf_char_t c) {
    if(!f) return CF_EPARAM;
    if(c != (cf_char_t)fputc(c, f)) {
        return CF_EFWRITE;
    }
    return CF_OK;
}

cf_errno_t  cf_file_gets(cf_file_t f, cf_char_t* buff, cf_size_t size) {
    if(!f || !buff) return CF_EPARAM;
    if(!fgets(buff, size, f)) {
        return CF_EFREAD;
    }
    return CF_OK;
}

cf_errno_t  cf_file_puts(cf_file_t f, const cf_char_t* buff) {
    if(!f || !buff) return CF_EPARAM;
    if(fputs(buff, f) <= 0) {
        return CF_EFWRITE;
    }
    return CF_OK;
}

cf_errno_t  cf_file_printf(cf_file_t f, const cf_char_t* fmtstr, ...) {
    va_list args;
    cf_int_t ret = 0;
    if(!f || !fmtstr) return CF_EPARAM;
    va_start(args, fmtstr);
    ret = vfprintf(f, fmtstr, args);
    va_end(args);
    return (ret == -1) ? CF_EFWRITE : CF_OK;
}

cf_errno_t  cf_file_scanf(cf_file_t f, const cf_char_t* fmtstr, ...) {
    va_list args;
    cf_int_t ret = 0;
    if(!f || !fmtstr) return CF_EPARAM;
    va_start(args, fmtstr);
    ret = vfscanf(f, fmtstr, args);
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
    if(_findnext(_dir->handle, &_dirent->fileinfo) == -1L) return CF_EEOF;
    cf_strcpy_s(dirinfo->name, sizeof(dirinfo->name), _dirent->fileinfo.name);
#else
    _dirent->pdir = readdir(_dir->dir);
    if(!_dirent->pdir) return CF_EEOF;
    cf_strcpy_s(dirinfo->name, sizeof(dirinfo->name), _dirent->pdir->d_name);
#endif
    return CF_OK;
}

cf_bool_t   cf_file_exist(const cf_char_t* filename) {
    if(!filename) return CF_FALSE;
#ifdef CF_OS_WIN
    return _access(filename, 0) == 0 ? CF_TRUE : CF_FALSE;
#else
    return access(filename, 0) == 0 ? CF_TRUE : CF_FALSE;
#endif
}

cf_errno_t  cf_file_rmdir(const cf_char_t* dirname) {
    if(!dirname) return CF_NOK;
#ifdef CF_OS_WIN
    return _rmdir(dirname) == 0 ? CF_OK : CF_NOK;
#else
    return rmdir(dirname) == 0 ? CF_OK : CF_NOK;
#endif
}

cf_errno_t  cf_file_remove(const cf_char_t* filename) {
    if(!filename) return CF_NOK;
    return remove(filename) == 0 ? CF_OK : CF_NOK;
}

cf_errno_t  cf_file_chdir(const cf_char_t* dirname) {
    if(!dirname) return CF_NOK;
#ifdef CF_OS_WIN
    return _chdir(dirname) == 0 ? CF_OK : CF_NOK;
#else
    return chdir(dirname) == 0 ? CF_OK : CF_NOK;
#endif
}

cf_errno_t  cf_file_getcwd(cf_char_t* cwd, cf_size_t size) {
    if(!cwd) return CF_EPARAM;
#ifdef CF_OS_WIN
    return _getcwd(cwd, size) ? CF_OK : CF_NOK;
#else
    return getcwd(cwd, size) ? CF_OK : CF_NOK;
#endif
}

cf_errno_t  cf_file_copy(const cf_char_t* src, const cf_char_t* dst) {
#ifndef CF_OS_WIN
    cf_int_t fd1 = 0;
    cf_int_t fd2 = 0;
    cf_int_t len;
    cf_char_t buff[1024]; 
#endif
    if(!src || !dst) return CF_EPARAM;
    
#ifdef CF_OS_WIN
    return CopyFile(src, dst, 0) ? CF_OK : CF_NOK; /* copy force */
#else
    fd1 = open(src,O_RDWR|O_CREAT);
    if(!fd1) return CF_NOK;

    fd2 = open(dst,O_RDWR|O_CREAT);
    if(!fd2) {
        close(fd1);
        return CF_NOK;
    }
    while((len = read(fd1, buff, sizeof(buff)))) {  
        write(fd2, buff, len);
    }
    close(fd1);
    close(fd2);
    return CF_OK;
#endif
}

cf_errno_t  cf_file_rename(const cf_char_t* src, const cf_char_t* dst) {
    if(!dst || !src) return CF_EPARAM;
    return rename(src, dst) == 0 ? CF_OK : CF_NOK;
}

// TODO: permisissions
cf_errno_t  cf_file_mkdir(const cf_char_t* path) {
    if(!path) return CF_EPARAM;
#ifdef CF_OS_WIN
    return _mkdir(path) == 0 ? CF_OK : CF_NOK;
#else
    return mkdir(path, 0755) == 0 ? CF_OK : CF_NOK;
#endif
}

cf_errno_t  cf_file_link(const cf_char_t* from_path, const cf_char_t* to_path) {
    if(!from_path || !to_path) return CF_EPARAM;
#ifdef CF_OS_WIN
    return CF_NOK;
#else
    return link(from_path, to_path) == 0 ? CF_OK : CF_NOK;
#endif
}

cf_errno_t  cf_file_unlink(const cf_char_t* pathname) {
    if(!pathname) return CF_EPARAM;
#ifdef CF_OS_WIN
    return _unlink(pathname) == 0 ? CF_OK : CF_NOK;
#else
    return unlink(pathname) == 0 ? CF_OK : CF_NOK;
#endif
}

cf_filetype_t cf_file_type(const cf_char_t* path) {
    struct stat st;
    if(!path) return CF_FILE_TYPE_UNKNOWN;

    if(stat(path, &st) != 0) return CF_FILE_TYPE_UNKNOWN;
    
    switch(st.st_mode & S_IFMT) {
    case S_IFSOCK:
        return CF_FILE_TYPE_SOCK;
    case S_IFLNK: 
        return CF_FILE_TYPE_LINK;
    case S_IFREG: 
        return CF_FILE_TYPE_REGULAR;
    case S_IFBLK: 
        return CF_FILE_TYPE_BLOCK;
    case S_IFDIR: 
        return CF_FILE_TYPE_DIR;
    case S_IFCHR: 
        return CF_FILE_TYPE_CHAR;
    case S_IFIFO: 
        return CF_FILE_TYPE_PIPE;
    default: 
        return CF_FILE_TYPE_NOT_DEF;
    }
}