#include <cf/types.h>
#include <cf/err.h>
#include <utils/zip.h>


#ifndef ZIP_LCL_FILE_HDR_SIG
#   define ZIP_LCL_FILE_HDR_SIG (0x04034b50)
#endif
#ifndef ZIP_DIR_HDR_SIG
#define ZIP_DIR_HDR_SIG (0x02014b50)
#endif
#ifndef ZIP_DIGITAL_HDR_SIG
#   define ZIP_DIGITAL_HDR_SIG (0x05054b50)
#endif
#ifndef ZIP_DIR_END_SIG
#   define ZIP_DIR_END_SIG (0x06054b50)
#endif

/**
      (see accompanying documentation for algorithm
      descriptions)

      0 - The file is stored (no compression)
      1 - The file is Shrunk
      2 - The file is Reduced with compression factor 1
      3 - The file is Reduced with compression factor 2
      4 - The file is Reduced with compression factor 3
      5 - The file is Reduced with compression factor 4
      6 - The file is Imploded
      7 - Reserved for Tokenizing compression algorithm
      8 - The file is Deflated
      9 - Enhanced Deflating using Deflate64(tm)
     10 - PKWARE Data Compression Library Imploding
     11 - Reserved by PKWARE
     12 - File is compressed using BZIP2 algorithm
    */
#define ZIP_COMPRESS_METHOD_STORED      0
#define ZIP_COMPRESS_METHOD_SHRUNK      1
#define ZIP_COMPRESS_METHOD_FACTOR_1    2
#define ZIP_COMPRESS_METHOD_FACTOR_2    3
#define ZIP_COMPRESS_METHOD_FACTOR_3    4
#define ZIP_COMPRESS_METHOD_FACTOR_4    5
#define ZIP_COMPRESS_METHOD_IMPLODED    6
#define ZIP_COMPRESS_METHOD_TOKENIZING  7
#define ZIP_COMPRESS_METHOD_DEFLATED    8
#define ZIP_COMPRESS_METHOD_DEFLATED64  9
#define ZIP_COMPRESS_METHOD_PKWARE      10
#define ZIP_COMPRESS_METHOD_RPKWARE     11
#define ZIP_COMPRESS_METHOD_BZIP2       12

/**
    local file header signature     4 bytes  (0x04034b50)
    version needed to extract       2 bytes
    general purpose bit flag        2 bytes
    compression method              2 bytes
    last mod file time              2 bytes
    last mod file date              2 bytes
    crc-32                          4 bytes
    compressed size                 4 bytes
    uncompressed size               4 bytes
    file name length                2 bytes
    extra field length              2 bytes

    file name (variable size)
    extra field (variable size)
 */
typedef struct {
    cf_uint32_t signature;
    cf_uint16_t ver_need;
    cf_uint16_t flag;
    cf_uint16_t method;
    cf_uint16_t mod_time;
    cf_uint16_t mod_date;
    cf_uint32_t crc32;
    cf_uint32_t compr_size;
    cf_uint32_t uncompr_size;
    cf_uint16_t fn_len;
    cf_uint16_t extra_len;
} zip_local_file_header_t;

/**
    crc-32                          4 bytes
    compressed size                 4 bytes
    uncompressed size               4 bytes
 */
typedef struct {
    cf_uint32_t crc32;
    cf_uint32_t compr_size;
    cf_uint32_t uncompr_size;
} zip_data_desc_t;

/**
    File header:
    central file header signature   4 bytes  (0x02014b50)
    version made by                 2 bytes
    version needed to extract       2 bytes
    general purpose bit flag        2 bytes
    compression method              2 bytes
    last mod file time              2 bytes
    last mod file date              2 bytes
    crc-32                          4 bytes
    compressed size                 4 bytes
    uncompressed size               4 bytes
    file name length                2 bytes
    extra field length              2 bytes
    file comment length             2 bytes
    disk number start               2 bytes
    internal file attributes        2 bytes
    external file attributes        4 bytes
    relative offset of local header 4 bytes

    file name (variable size)
    extra field (variable size)
    file comment (variable size)
    */
typedef struct {
    cf_uint32_t signature;
    cf_uint16_t ver_made;
    cf_uint16_t ver_need;
    cf_uint16_t flag;
    cf_uint16_t method;
    cf_uint16_t mod_time;
    cf_uint16_t mod_date;
    cf_uint32_t crc32;
    cf_uint32_t compr_size;
    cf_uint32_t uncompr_size;
    cf_uint16_t fn_len;
    cf_uint16_t extra_len;
    cf_uint16_t comment_len;
    cf_uint16_t disknum_start;
    cf_uint16_t int_file_attr;
    cf_uint32_t ext_file_attr;
    cf_uint32_t offset;
} zip_dir_file_header_t;

/**
    Digital signature:

    header signature                4 bytes  (0x05054b50)
    size of data                    2 bytes
    signature data (variable size)
    */
typedef struct {
    cf_uint32_t sig;
    cf_uint16_t size;
} zip_dig_sig_t

/**
    End of central directory record:

    end of central dir signature    4 bytes  (0x06054b50)
    number of this disk             2 bytes
    number of the disk with the
    start of the central directory  2 bytes
    total number of entries in the
    central directory on this disk  2 bytes
    total number of entries in
    the central directory           2 bytes
    size of the central directory   4 bytes
    offset of start of central
    directory with respect to
    the starting disk number        4 bytes
    .ZIP file comment length        2 bytes
    .ZIP file comment       (variable size)
    */
typedef struct {
    cf_uint32_t sig;
    cf_uint16_t disk_num;
    cf_uint16_t disk_num_with_start;
    cf_uint16_t entries_disk;
    cf_uint16_t entries_dir;
    cf_uint16_t central_dir;
    cf_uint32_t dir_size;
    cf_uint32_t offset;
    cf_uint16_t comment_len;
} zip_dir_file_end_t