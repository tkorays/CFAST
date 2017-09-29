#include "cfast/cmd.h"
#include "stdlib.h"

CFAST_RET CFAST_CMD_Init(CFAST_CMD_Env** env, CFAST_CMD_Node* root) {
    if(!env || !root) return CFAST_PARAM_ERR;

    *env = (CFAST_CMD_Env*)malloc(sizeof(CFAST_CMD_Env));
    if(!*env) return CFAST_RET_ALLOC_ERR;

    *env->root = root;
    memset(*env->out, 0, CFAST_CMD_OUT_MAX_LEN);
    return CFAST_RET_OK;
}

CFAST_RET CFAST_CMD_Uninit(CFAST_CMD_Env** env) {
    if(!env || !*env) return CFAST_RET_PARAM_ERR;
    free(*env);
    *env = CFAST_NULL;
    return CFAST_RET_OK;
}

CFAST_RET CFAST_CMD_Input(CFAST_CMD_Env* env, const CFAST_CHAR* in, CFAST_CMD_EVENT evt) {
    CFAST_CHAR* pCh = CFAST_NULL;
    CFAST_UINT indexArray[256]= {0};
    CFAST_UINT i;
    if(!env || !in) return CFAST_RET_PARAM_ERR;


    return CFAST_RET_OK;
}