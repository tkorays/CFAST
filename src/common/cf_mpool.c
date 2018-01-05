#include "cfast/cf_mpool_if.h"
#include "cfast/cf_mem_if.h"

#define CF_ALIGN_SIZE 4
#define CF_ADDR_ALIGN(d, a) (((d)+(a-1)) & ~(a-1))

typedef struct cf_mpool_block {
    cf_char_t*              start;  /** 空闲内存起始地址 */
    cf_char_t*              end;    /** 内存结束地址 */
    struct cf_mpool_block*  next;   /** 下一个内存块地址 */
} cf_mpool_block_t;

struct cf_mpool {
    cf_size_t           blk_size;   /** 一个内存分配块大小 */
    cf_size_t           blk_num;    /** 内存分配块个数 */
    cf_mpool_block_t*   head;       /** 内存块头指针 */
    cf_mpool_block_t*   ptr;        /** 当前申请块指针 */
};

cf_mpool_block_t*   cf_mpool_block_create(cf_size_t blk_size);
cf_void_t           cf_mpool_block_destroy(cf_mpool_block_t* block);

cf_mpool_t* cf_mpool_create(cf_size_t blk_size) {
    struct cf_mpool* pool = CF_NULL_PTR;
    if(blk_size == 0) return CF_NULL_PTR;

    pool = cf_malloc(sizeof(struct cf_mpool));
    if(!pool) return CF_NULL_PTR;

    pool->blk_size = blk_size;

    /* 默认申请一块内存分配块 */
    pool->head = cf_mpool_block_create(blk_size);
    if(!pool->head) {
        cf_free(pool);
        return CF_NULL_PTR;
    }

    pool->ptr = pool->head;
    pool->blk_num = 1;

    return (cf_mpool_t*)pool;
}

cf_void_t   cf_mpool_destroy(cf_mpool_t* pool) {
    cf_mpool_block_t *it, *next;
    if(!pool) return ;

    it = pool->head;
    while(it) {
        next = it->next;
        cf_mpool_block_destroy(it);
        it = next;
    }

    cf_free(pool);
}

cf_void_t*  cf_mpool_alloc(cf_mpool_t* pool, cf_size_t size) {
    cf_char_t* addr = CF_NULL_PTR;
    cf_mpool_block_t* blk = CF_NULL_PTR;
    struct cf_mpool* p = (struct cf_mpool*)pool;
    if(!p || size == 0 || size > p->blk_size) return CF_NULL_PTR;

    /* 字节对齐 */
    addr = (cf_char_t*)CF_ADDR_ALIGN((cf_uintptr_t)p->ptr->start, CF_ALIGN_SIZE);

    if((p->ptr->end - addr) < size) {
        /* 当前内存分配块不够，则创建一个新的块 */
        blk = cf_mpool_block_create(p->blk_size);
        if(!blk) return CF_NULL_PTR;

        p->ptr->next = blk;
        p->ptr = blk;

        /* 块的首地址已经对齐 */
        addr = p->ptr->start;
        p->ptr->start = addr + size;
        p->blk_num++;
    } else {
        p->ptr->start = addr + size;
    }

    return addr;
}

cf_mpool_block_t* cf_mpool_block_create(cf_size_t blk_size) {
    cf_mpool_block_t* blk = CF_NULL_PTR;
    cf_char_t* addr = CF_NULL_PTR;

    addr = (cf_char_t*)cf_malloc(blk_size + sizeof(cf_mpool_block_t));
    if(!addr) return CF_NULL_PTR;

    blk = (cf_mpool_block_t*)addr;
    /* 起始地址字节对齐，可能存在浪费字节 */
    blk->start = (cf_char_t*)CF_ADDR_ALIGN((cf_uintptr_t)addr + sizeof(cf_mpool_block_t), CF_ALIGN_SIZE);
    blk->end = addr + sizeof(cf_mpool_block_t) + blk_size;
    blk->next = CF_NULL_PTR;
    return blk;
}

cf_void_t cf_mpool_block_destroy(cf_mpool_block_t* block) {
    if(block) {
        cf_free(block);
    }
}
