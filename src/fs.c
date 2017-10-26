#include "cfast/fs.h"
#include <string.h>
#include <stdlib.h>

#ifdef CF_OS_WIN
#include <io.h>
#include <direct.h>
#else
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#endif

char* cf_fs_getcwd(char* buf, cf_size_t buf_size) {
    if(!buf || buf_size == 0) return CF_NULL;
    return getcwd(buf, buf_size);
}

char* cf_fs_path_join(char* buf, cf_size_t buf_size, char* path1, char* path2) {
    int i, j;
    if(!buf || buf_size == 0 || !path1 || !path2) return CF_NULL;
    i = strlen(path1);
    j = strlen(path2);

    if(i == 0) return CF_NULL;
    i--;
    while(path1[i] == CF_FS_PATH_SPLITER) i--;
    while(*path2 == CF_FS_PATH_SPLITER) path2++;
    if(i+strlen(path2)+3 > buf_size) return CF_NULL;
    strncpy(buf, path1, i+1);
    buf[i+1] = CF_FS_PATH_SPLITER;
    strncpy(buf + i + 2, path2, strlen(path2)+1);
    return buf;
}

char* cf_fs_realpath(char* buf, cf_size_t buf_size, char* path) {
    return CF_NULL;
}

cf_list_t* cf_fs_list_dir(char* path)
{
    cf_list_t *li = CF_NULL;
    cf_node_t *node = CF_NULL;
#ifdef CF_OS_WIN
    // TODO: not test yet
    _finddata_t	fileinfo;
    intptr_t hFile;
    char fullpath[512];
    int len = 0;
    if(!path) return CF_NULL;

    len = strlen(path);
    (void)memset_s(fullpath, 0, 512);
    strncpy(fullpath, path, len);
    fullpath[len] = CF_FS_PATH_SPLITER;
    fullpath[len+1] = '*';
    fullpath[len+2] = '\0';

    hFile = _findfirst(path, &fileinfo);
    if(-1 == hFile) return CF_NULL;

    do {
        LISTCMD_INFO TmpDirInfo = {0};
        TransFileInfo(fileinfo, TmpDirInfo);
        FilesPathVector.push_back(TmpDirInfo);
    } while(_findnext(hFile, &fileinfo) == 0);
#else
    DIR *dir = CF_NULL;
    struct dirent *ent = CF_NULL;
    struct stat fst;
    if(!path || !(dir = opendir(path))) return CF_NULL;

    li = cf_list_new();
    if(!li) return CF_NULL;

    while((ent = readdir(dir)) != CF_NULL) {
        if(ent->d_type & DT_DIR) {
            if(strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0) continue;
            node = cf_list_new_node(li, strlen(ent->d_name)+2);
            if(!node) {
                cf_list_free(&li);
                return CF_NULL;
            }
            (void)memcpy(node->data, ent->d_name, strlen(ent->d_name)+1);
            *((char*)(node->data)+node->size-2) = '/';
            *((char*)(node->data)+node->size-1) = '\0';
        } else {
            node = cf_list_new_node(li, strlen(ent->d_name)+1);
            if(!node) {
                cf_list_free(&li);
                return CF_NULL;
            }
            (void)memcpy(node->data, ent->d_name, strlen(ent->d_name)+1);
        }
    }
#endif
    return li;
}

/*
#include <stdio.h>
cf_bool_t pfn(cf_node_t* n, void* d)
{
    printf("%s\n", (char*)n->data);
    return cf_false;
}

int main() {
    cf_list_t* li = cf_fs_list_dir(".");
    cf_node_t* n = cf_node_new(256);
    cf_node_t* t = cf_node_new(256);
    if(!cf_fs_getcwd(n->data, n->size)) {
        printf("get cwd fail.\n");
    } else printf("ccccwd: %s\n", (char*)n->data);

    cf_fs_path_join(t->data, t->size, "/fuck/cao/tt///", "/nimei/hahah/biu.txt");
    printf("%s\n", t->data);

    cf_list_foreach(li, pfn, CF_NULL);
    cf_list_free(&li);
}
*/
