#include "cfast/tinympool.h"
#include <stdlib.h>

#define MEM_POOL_BLOCK_DEFAULT_SIZE 1024

CFAST_TMPOOL_Block *CFAST_TMPOOL_BlockCreate();
void CFAST_TMPOOL_BlockDestroy(CFAST_TMPOOL_Block *blk);
CFAST_SIZE CFAST_TMPOOL_BlockNum(CFAST_TMPOOL_Pool *pool);

CFAST_TMPOOL_Block* CFAST_TMPOOL_BlockCreate(){
    CFAST_CHAR*           m;
    CFAST_TMPOOL_Block*    blk;

    m = (CFAST_CHAR*)malloc(MEM_POOL_BLOCK_DEFAULT_SIZE + sizeof(CFAST_TMPOOL_Block));
    if(!m){
        return 0;
    }

    blk = (CFAST_TMPOOL_Block*)m;
    blk->last = m + sizeof(CFAST_TMPOOL_Block);
    blk->end = m + MEM_POOL_BLOCK_DEFAULT_SIZE + sizeof(CFAST_TMPOOL_Block);
    blk->next = 0;
    return blk;
}

void CFAST_TMPOOL_BlockDestroy(CFAST_TMPOOL_Block* blk){
    if(blk){
        free(blk);
    }
}

CFAST_SIZE CFAST_TMPOOL_BlockNum(CFAST_TMPOOL_Pool* pool){
    CFAST_TMPOOL_Block*    blk;
    CFAST_SIZE          cnt = 0;
    if(!pool){
        return 0;
    }
    blk = pool->head;
    while(blk){
        cnt++;
        blk = blk->next;
    }
    return cnt;
}

CFAST_TMPOOL_Pool* mem_pool_create(){
    CFAST_TMPOOL_Pool*     pool;
    CFAST_TMPOOL_Block*    blk;

    pool = (CFAST_TMPOOL_Pool*)malloc(sizeof(CFAST_TMPOOL_Pool));
    if(!pool){
        return 0;
    }

    blk = CFAST_TMPOOL_BlockCreate();
    if(!blk){
        free(pool);
        return 0;
    }

    pool->head      = blk;
    pool->current   = blk;

    return pool;
}


void mem_pool_destroy(CFAST_TMPOOL_Pool* pool){
    CFAST_TMPOOL_Block*     cur;
    CFAST_TMPOOL_Block*    next;

    if(!pool){
        return;
    }

    cur = pool->head;
    while(cur){
        next = cur->next;
        CFAST_TMPOOL_BlockDestroy(cur);
        cur = next;
    }

    free(pool);
}

/* 为了简单，没有考虑到地址对齐 */
void* CFAST_TMPOOL_Alloc(CFAST_TMPOOL_Pool* pool, CFAST_SIZE n){
    CFAST_CHAR*           m;
    int             is_size_valid;
    int             left_size;
    CFAST_TMPOOL_Block*    blk;
    CFAST_UINTPTR       addr;

    is_size_valid = ( n<=0 )||(n > MEM_POOL_BLOCK_DEFAULT_SIZE);
    if(!pool || !pool->current || is_size_valid){
        return 0;
    }

    addr = CFAST_ADDR_ALIGN((CFAST_UINTPTR)pool->current->last, CFAST_ALIGN_SIZE);
    left_size = (CFAST_UINTPTR)pool->current->end - addr; /* may <= 0 */
    if(n > left_size){
        blk = CFAST_TMPOOL_BlockCreate();
        if(!blk){
            return 0;
        }

        pool->current->next = blk;
        pool->current = blk;
        m = blk->last;
        blk->last += n;
        return m;
    }

    m = pool->current->last;
    pool->current->last += n;
    return m;
}

