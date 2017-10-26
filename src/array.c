#include "cfast/array.h"

typedef struct ARRAY_Node {
    CFAST_VOID* data;
    CFAST_SIZE  size;
    struct ARRAY_Node* next;
} CFAST_ARRAY_Node;

typedef struct ARRAY_MemNode {
    CFAST_VOID* data;
    struct ARRAY_MemNode* next;
} CFAST_ARRAY_MemNode;

typedef struct {
    CFAST_SIZE count;       /* 保存元素个数 */
    CFAST_SIZE section;     /* 每段元素个数 */
    CFAST_SIZE size;        /* 可容纳总元素，可自动增长 */
    
    ARRAY_Node *first;      /* 数组元素 */ 
    CFAST_ARRAY_MemNode mm; /* 动态内存管理链表 */
} CFAST_ARRAY;


#define CFAST_ARRAY(T, c) 
#define CFAST_ARRAY_IN(arr, idx) \
    (arr)->i = 0,\
    (arr)->tmp = (arr)->first,\
    for((arr)->i = 0, (arr)->tmp = (arr)->first; (arr)->tmp && i != idx; (arr)->tmp = (arr)->tmp->next) ;\
    ((arr)->i >= (arr)->count || !(arr)->tmp ? 0 : (arr)->tmp)

CFAST_ARRAY* CFAST_ARRAY_Create(CFAST_SIZE typeSize, CFAST_SIZE allocStep) {

}

CFAST_VOID CFAST_ARRAY_Destroy(CFAST_ARRAY** arr) {
    if(!arr || !*arr) return ;

}

CFAST_VOID* CFAST_ARRAY_IN(CFAST_ARRAY* arr, CFAST_SIZE idx) {
    ARRAY_Node *tmp; 
    CFAST_SIZE i;  
    if(!arr || idx >= arr->count) return CFAST_NULL;
    for(i = 0, tmp = arr->first; tmp && i != idx; tmp = tmp->next) ;
    return (i >= arr->count || tmp ? CFAST_NULL : tmp);
}