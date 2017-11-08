#ifndef __CFAST_ZIP_H__
#define __CFAST_ZIP_H__

#include "cftype.h"

#define CF_ZIP_FH_SIG               0x04034b50
#define CF_ZIP_EFS_SIG              0x08064b50
#define CF_ZIP_DIR_SIG              0x02014b50
#define CF_ZIP_DIG_SIG              0x05054b50
#define CF_ZIP64_DIR_END_RCD_SIG    0x06064b50
#define CF_ZIP64_DIR_END_LOC_SIG    0x07064b50
#define CF_ZIP_DIR_END_RCD_SIG      0x06054b50

/**
0 - MS-DOS and OS/2 (FAT / VFAT / FAT32 file systems)
1 - Amiga                     2 - OpenVMS
3 - Unix                      4 - VM/CMS
5 - Atari ST                  6 - OS/2 H.P.F.S.
7 - Macintosh                 8 - Z-System
9 - CP/M                     10 - Windows NTFS
11 - MVS (OS/390 - Z/OS)      12 - VSE
13 - Acorn Risc               14 - VFAT
15 - alternate MVS            16 - BeOS
17 - Tandem                   18 - OS/400
19 - OS/X (Darwin)            20 thru 255 - unused
 */

typedef struct {
    uint32_t    signature;
    uint16_t    version_extract;
    uint16_t    general_flag;
    uint16_t    compress_method;
    uint16_t    last_mod_time;
    uint16_t    last_mod_date;
    uint32_t    crc32;
    uint32_t    comp_size;
    uint32_t    uncomp_size;
    uint16_t    filename_len;
    uint16_t    extra_field_len;
    /* filename */
    /* extra field */
} cf_zip_file_header_t;

typedef struct {
    uint32_t    crc32;
    uint32_t    comp_size;
    uint32_t    uncomp_size;
} cf_zip_data_desc_t;

typedef struct {
    uint32_t    signature;
    uint32_t    extra_field_len;
    /* extra data */
} cf_zip_efs_t;

typedef struct {
    uint32_t    signature;
    uint16_t    version;
    uint16_t    version_extract;
    uint16_t    general_flag;
    uint16_t    compress_method;
    uint16_t    last_mod_time;
    uint16_t    last_mod_date;
    uint16_t    crc32;
    uint32_t    comp_size;
    uint32_t    uncomp_size;
    uint16_t    filename_len;
    uint16_t    extra_field_len;
    uint16_t    file_comment_len;
    uint16_t    disk_num_start;
    uint16_t    int_file_attrs;
    uint32_t    ext_file_attrs;
    uint32_t    offset;
    /* filename */
    /* extra field */
    /* file comment */
} cf_zip_dir_t;

typedef struct {
    uint32_t    signature;
    uint16_t    size;
    /* signature data */
} cf_zip_dig_sig_t;

#endif /* __CFAST_ZIP_H__ */
