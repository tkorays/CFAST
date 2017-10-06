#ifndef __CFAST_H__
#define __CFAST_H__

#include <stdint.h>

#define CFAST_NULL    0


#define CFAST_MIN(a, b) ((a) > (b) ? (b) : (a))
#define CFAST_MAX(a, b) ((a) > (b) ? (a) : (b))

#define CFAST_CHAR    char 
#define CFAST_UCHAR   unsigned char 
#define CFAST_INT8    char 
#define CFAST_UINT8   unsigned char
#define CFAST_INT16   short
#define CFAST_UINT16  unsigned short
#define CFAST_SHORT   short
#define CFAST_INT     int
#define CFAST_UINT    unsigned int
#define CFAST_INT32   int32_t
#define CFAST_UINT32  uint32_t

#define CFAST_VOID    void
#define CFAST_PVOID   void*

#define CFAST_HANDLE  void*
#define CFAST_ID      uint32_t

#define CFAST_SIZE    uint32_t

#define CFAST_BOOL    uint32_t
#define CFAST_TRUE    1
#define CFAST_FALSE   0

#define CFAST_UINTPTR uintptr_t

#define CFAST_Malloc(n) malloc((n))
#define CFAST_Free(a) free((a))

#define CFAST_SIZEOF_ARRAY(arr, t) (sizeof(arr)/sizeof(t))

#define CFAST_ALIGN_SIZE 4
#define CFAST_ADDR_ALIGN(d,a) (((d)+(a-1)) & ~(a-1))

typedef enum {
    CFAST_RET_OK,
    CFAST_RET_ERROR,
    CFAST_RET_PARAM_ERROR,
    CFAST_RET_ALLOC_ERR
} CFAST_RET;

typedef enum {
    CFAST_LANG_EN,
    CFAST_LANG_CN,
    CFAST_LANG_OTHER
} CFAST_LANG;

typedef enum {
    CFAST_PARAM_NEEDLESS,
    CFAST_PARAM_OPTIONAL,
    CFAST_PARAM_ESSENTIAL
} CFAST_PARAM_NEED;

#define CFAST_LANG_IS_EN(lang) ((lang) == CFAST_LANG_EN)
#define CFAST_LANG_IS_CN(lang) ((lang) == CFAST_LANG_CN)

typedef struct {
    CFAST_CHAR* data;
    CFAST_SIZE  len;
} CFAST_String;


#endif /* __CFAST_H__ */
