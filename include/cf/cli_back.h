#ifndef __CFAST_CLI_H__
#define __CFAST_CLI_H__

#include "cfast_def.h"

#define CFAST_CLI_MAX_NAME_LEN  32
#define CFAST_CLI_MAX_OPT_NUM   32
#define CFAST_CLI_MAX_SUB_CMD_NUM 5
#define CFAST_CLI_MAX_ARG_NUM   32
#define CFAST_CLI_VERSION_SIZE 32
#define CFAST_CLI_MAX_PARAM_SIZE 256

#define CFAST_CLI_MAX_DESC_SIZE 256
#define CFAST_CLI_MAX_BUF_SIZE  1024

//typedef struct CFAST_CLI_CmdOptResult;
struct CFAST_CLI_CmdOptResult_S;
typedef CFAST_INT(*CFAST_CLI_PrintFunc)(CFAST_CHAR* fmtStr, ...);
typedef CFAST_VOID(*CFAST_CLI_CmdProcFunc)(struct CFAST_CLI_CmdOptResult_S* optInfo, CFAST_SIZE num);


/* 配置项 */
typedef struct {
    CFAST_ID    id;         /* 配置项id */
    CFAST_CHAR* shortName;  /* 短名称 */
    CFAST_CHAR* longName;   /* 长名称 */
    CFAST_BOOL  essential;  /* 是否必需 */
    CFAST_BOOL  needParam;  /* 是否需要参数 */
    CFAST_CHAR* helpEn;     /* 英文帮助 */
    CFAST_CHAR* helpLocal;  /* 本地语言帮助，如中文 */
} CFAST_CLI_CmdOptions;

/* 命令 */
typedef struct CFAST_CLI_Cmd_S{
    CFAST_ID                id;         /* 命令id */
    CFAST_CHAR*             name;       /* 命令名称 */
    struct CFAST_CLI_Cmd_S* subCmd;     /* 子命令 */
    CFAST_SIZE              subCmdNum;  /* 子命令个数 */
    CFAST_CLI_CmdOptions*   option;     /* 配置选项 */
    CFAST_SIZE              optionNum;  /* 配置选项个数 */
    CFAST_CLI_CmdProcFunc   procFunc;   /* 命令处理函数 */
    CFAST_CHAR*             helpEn;     /* 英文帮助 */
    CFAST_CHAR*             helpLocal;  /* 本地语言帮助，如中文 */
} CFAST_CLI_Cmd;

/* 配置项解析结果 */
typedef struct CFAST_CLI_CmdOptResult_S {
    CFAST_ID    id;     /* 配置编号 */
    CFAST_CHAR* param;  /* 解析的参数，不一定需要 */
} CFAST_CLI_CmdOptResult;

/* 命令行环境 */
typedef struct {
    CFAST_LANG          lang;       /* 命令行语言 */
    CFAST_CLI_PrintFunc printFunc;  /* 打印函数 */
    CFAST_CLI_Cmd*      cmd;        /* 根命令 */
    CFAST_BOOL          skipRoot;   /* 不关系根命令名称是否匹配 */
} CFAST_CLI_Env;

CFAST_CLI_Env* CFAST_CLI_CmdInit(CFAST_LANG lang, CFAST_CLI_PrintFunc func, CFAST_CLI_Cmd* cmd, CFAST_BOOL skipRoot);
CFAST_VOID CFAST_CLI_CmdUnInit(CFAST_CLI_Env** env);
CFAST_VOID CFAST_CLI_CmdHelp(CFAST_CLI_Env* env, CFAST_CLI_Cmd* cmd, CFAST_CHAR* rootCmd);
CFAST_VOID CFAST_CLI_CmdRun(CFAST_CLI_Env* env, CFAST_SIZE argc, CFAST_CHAR** argv);

#endif /* __CFAST_CLI_H__ */
