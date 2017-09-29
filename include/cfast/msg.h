#ifndef __CFAST_MSG_H__
#define __CFAST_MSG_H__

#include "cfast_def.h"

typedef enum {
    CFAST_SYNC_MSG,
    CFAST_ASYNC_MSG
} CFAST_MSG_TYPE;

typedef struct {
    CFAST_ID            msgid;
    CFAST_ID            receiver;
    CFAST_UINT32        param1;
    CFAST_UINT32        param2;
    CFAST_UINT32        param3;
    CFAST_PVOID         data;
    CFAST_UINT32        size;
} CFAST_Msg;

CFAST_RET CFAST_SendMsg(CFAST_Msg* msg, CFAST_MSG_TYPE type);

#endif /* __CFAST_MSG_H__ */
