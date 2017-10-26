#ifndef __CF_FS_H__
#define __CF_FS_H__

#include "cftype.h"
#include "list.h"

#ifdef CF_OS_WIN
#define CF_FS_PATH_SPLITER '\\'
#else
#define CF_FS_PATH_SPLITER '/'
#endif

char* cf_fs_getcwd(char* buf, cf_size_t buf_size);
cf_list_t* cf_fs_list_dir(char* path);


#endif /* __CF_FS_H__ */