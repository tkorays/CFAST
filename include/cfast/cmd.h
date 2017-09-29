#ifndef __CFAST_CMD_H__
#define __CFAST_CMD_H__

#include "cfast_def.h"

#define CFAST_CMD_NAME_MAX_LEN 32
#define CFAST_CMD_DESC_MAX_LEN 256
#define CFAST_CMD_IN_MAX_LEN  1024
#define CFAST_CMD_OUT_MAX_LEN 1024
#define CFAST_CMD_ID CFAST_UINT32

typedef enum {
    CFAST_CMD_PRESS_ENTER,
    CFAST_CMD_PRESS_TAB,
    CFAST_CMD_PRESS_BLANK
} CFAST_CMD_EVENT;

typedef CFAST_RET (*CFAST_CMD_ProcFunc)(CFAST_CHAR* in, CFAST_CHAR* out);

typedef struct _CFAST_CMD_Node {
    CFAST_CHAR              name[CFAST_CMD_NAME_MAX_LEN];       /* command name */
    CFAST_CHAR              desc[CFAST_CMD_DESC_MAX_LEN];       /* description */
    CFAST_CHAR              desc_cn[CFAST_CMD_DESC_MAX_LEN];    /* description */
    CFAST_CMD_ProcFunc      proc;                               /* process function */
    struct _CFAST_CMD_Node* child;                              /* child commands */
    CFAST_SIZE              size;                               /* size of child commands */
} CFAST_CMD_Node;

typedef struct {
    CFAST_CMD_Node* root;                       /* fast node */
    CFAST_CHAR      out[CFAST_CMD_OUT_MAX_LEN]; /* output buffer */
} CFAST_CMD_Env;

CFAST_RET CFAST_CMD_Init(CFAST_CMD_Env** env, CFAST_CMD_Node* root);
CFAST_RET CFAST_CMD_Uninit(CFAST_CMD_Env** env);
CFAST_RET CFAST_CMD_Input(CFAST_CMD_Env* env, const CFAST_CHAR* in, CFAST_CMD_EVENT evt);

#endif /* __CFAST_H__ */
