#ifndef __CFAST_ARRAY_H__
#define __CFAST_ARRAY_H__

#include "cfast_def.h"

typedef struct CFAST_ARRAY_Node_S{
    CFAST_VOID* data;
    CFAST_SIZE size;
    CFAST_ARRAY_Node_S* next;
} CFAST_ARRAY_Node;

typedef struct {
    CFAST_ARRAY_Node_S* first;
    CFAST_SIZE num;
} CFAST_ARRAY;

CFAST_ARRAY* CFAST_ARRAY_Create(CFAST_SIZE size);
CFAST_VOID CFAST_ARRAY_Destroy(CFAST_ARRAY* array);
CFAST_RET CFAST_ARRAY_Add(CFAST_ARRAY* array, CFAST_VOID* data, CFAST_SIZE size);
CFAST_RET CFAST_ARRAY_Delete(CFAST_ARRAY* array, CFAST_SIZE index);
CFAST_ARRAY_Node* CFAST_ARRAY_Get(CFAST_ARRAY* array, CFAST_SIZE index);


#endif /* __CFAST_ARRAY_H__ */
