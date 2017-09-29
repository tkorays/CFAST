#ifndef __CFAST_TINY_MPOOL__
#define __CFAST_TINY_MPOOL__

#include "cfast_def.h"

typedef struct CFAST_TMPOOL_Block_S CFAST_TMPOOL_Block;
typedef struct CFAST_TMPOOL_Pool_S CFAST_TMPOOL_Pool;

struct CFAST_TMPOOL_Block_S {
    CFAST_CHAR *last;	         /* 空闲内存start */
    CFAST_CHAR *end;          /* 该block最后地址 */
    CFAST_TMPOOL_Block *next;  /* 下一个block指针 */
};

struct CFAST_TMPOOL_Pool_S {
    CFAST_TMPOOL_Block *head;			/* 首个block */
    CFAST_TMPOOL_Block *current;      /* 当前可分配内存block */
};

/* 用户接口 */ 
CFAST_TMPOOL_Pool *CFAST_TMPOOL_Create();
void CFAST_TMPOOL_Destroy(CFAST_TMPOOL_Pool *pool);
void *CFAST_TMPOOL_Alloc(CFAST_TMPOOL_Pool *pool, CFAST_SIZE n); /* 申请的内存没有初始化为0 */

#endif /* __CFAST_TINY_MPOOL__ */
