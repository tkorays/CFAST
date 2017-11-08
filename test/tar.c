#include <stdio.h>
#include <stdlib.h>
#include "../include/cfast/cftype.h"
#include "../include/cfast/list.h"

#define CF_TAR_HDR_SIZE 512


typedef struct {
    char name[100];
    char mode[8];
    char uid[8];
    char gid[8];
    char size[12];
    char mtime[12];
    char chksum[8];
    char linkflag;
    char linkname[100];
    char magic[8]; /* "ustar" */
    char version[2];
    char uname[32];
    char gname[32];
    char devmajor[8];
    char devminor[8];
    char prefix[155];
    char padding[12];
} cf_tar_head_t;

#define _CF_MAGIC "ustar  "
#define _CF_LINK_OLDNORMAL  '\0'
#define _CF_LINK_NORMAL     '0'
#define _CF_LINK_LINK       '1'
#define _CF_LINK_SYMLINK    '2'
#define _CF_LINK_CHR        '3'
#define _CF_LINK_BLK        '4'
#define _CF_LINK_DIR        '5'
#define _CF_LINK_FIFO       '6'
#define _CF_LINK_CONTIG     '7'    


typedef struct {
    FILE *file;
} cf_tar_t;

cf_tar_t* cf_tar_new(const char* path) {
    cf_tar_t *tar = CF_NULL;
    if( !path || !(tar = (cf_tar_t *)malloc(sizeof(cf_tar_t))) ) {
        return CF_NULL;
    }

    tar->file = fopen(path, "wb");
    if(!tar->file) {
        free(tar);
        return CF_NULL;
    }

    return tar;
}

void cf_tar_add(cf_tar_t* tar, const char* path, void* buf, cf_size_t size) {
    if(!tar || !tar->file || !path || !buf || size == 0) {
        return ;
    }


}
