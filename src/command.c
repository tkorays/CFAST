#include "cfast/command.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* macro start */
#define CMD_PRINT(env, fmtStr) \
    do {\
        if((env)->outCallback) ((env)->outCallback)((fmtStr));\
    } while(0)
#define CMD_PRINT_EX(env, buf, fmtStr, ...)\
    do {\
        if((env)->outCallback) {\
            memset(buf, 0, CFAST_CMD_MAX_BUF_SIZE); \
            snprintf(buf, CFAST_CMD_MAX_BUF_SIZE, fmtStr, ##__VA_ARGS__);\
            (env)->outCallback(buf);\
        }\
    } while(0)

#define CMD_BUF_PRINT_HELPER(buf, cnt, wrtCnt, printFunc, fmtStr, ...) \
    do {\
        wrtCnt = snprintf(buf + cnt, CFAST_CMD_MAX_BUF_SIZE, fmtStr, ##__VA_ARGS__);\
        if(wrtCnt < 0) {\
            printFunc(buf);\
            cnt = 0;\
        } else {\
            cnt += wrtCnt; \
        }\
    }while(0)

#define CMD_BUF_FLUSH_HELPER(buf, cnt, printFunc) do {\
    printFunc(buf); cnt = 0;\
    } while(0)

#define CMD_OPTION_EXIST(pstCmd) ((pstCmd)->optionsNum && (pstCmd)->options)
#define CMD_SUBCMD_EXIST(pstCmd) ((pstCmd)->subCmdNum && (pstCmd)->subCmd)

#define CMD_LANG_IS_CN(env) ((env)->lang == CFAST_LANG_CN)
/* macro end */

CFAST_VOID _DefaultOutCallProc(CFAST_CHAR* output) {
    printf("%s", output);
}

CFAST_RET CFAST_CMD_Init(CFAST_CMD_Env* env, CFAST_CMD_Command* cmdDef, CFAST_SIZE cmdNum) {
    if(!env || !cmdDef) return CFAST_RET_PARAM_ERROR;
    env->cmds = cmdDef;
    env->cmdsNum = cmdNum;
    env->outCallback = _DefaultOutCallProc;
    env->lang = CFAST_LANG_CN;
    return CFAST_RET_OK;
}

CFAST_RET CFAST_CMD_UnInit(CFAST_CMD_Env* env) {

    return CFAST_RET_OK;
}

CFAST_VOID CFAST_CMD_SetOutFunc(CFAST_CMD_Env* env, CFAST_CMD_OutputCallback outCallback)
{
    if(!env || !outCallback) return;
    env->outCallback = outCallback;
}

CFAST_VOID CFAST_CMD_PrintHelp(CFAST_CMD_Env* env, CFAST_CMD_Command* cmd) {
    CFAST_CHAR buf[CFAST_CMD_MAX_BUF_SIZE] = {0};
    CFAST_INT i = 0;
    CFAST_INT cnt = 0;
    CFAST_INT wrtCnt;
    CFAST_CMD_Option* opt = CFAST_NULL;
    CFAST_CMD_Command* subCmd = CFAST_NULL;
    if(!env || !cmd) return ;
    if(!env->outCallback) return ;

#define CMD_HELP_PRINT(fmtStr, ...) \
    CMD_BUF_PRINT_HELPER(buf, cnt, wrtCnt, env->outCallback, fmtStr, ##__VA_ARGS__)
#define CMD_HELP_FLUSH_BUF() CMD_BUF_FLUSH_HELPER(buf, cnt, env->outCallback)
#define CMD_DISPLAY_MARGIN_SIZE 40

    CMD_HELP_PRINT("usage: %s", cmd->name);

    /* overview options */
    if(CMD_OPTION_EXIST(cmd)) {
        for(i = 0; i < cmd->optionsNum; i++) {
            opt = &cmd->options[i];
            CMD_HELP_PRINT(" %s%s%s", 
                (opt->isOptional ? "[" : ""),
                (!opt->simpleName ? "--" : "-"),
                (!opt->simpleName ? opt->name : "")
            );
            if(opt->simpleName) CMD_HELP_PRINT("%c", opt->simpleName);
            CMD_HELP_PRINT("%s", (opt->isOptional ? "]" : ""));
        }
    }

    CMD_HELP_PRINT("\n");

    /* display subcommands */
    CMD_HELP_PRINT("subcommands:\n");
    if(CMD_SUBCMD_EXIST(cmd)) {
        for(i = 0; i < cmd->subCmdNum; i++) {
            subCmd = &cmd->subCmd[i];
            CMD_HELP_PRINT("\t%s: %s\n", subCmd->name, 
            CMD_LANG_IS_CN(env) ? subCmd->helpCn : subCmd->helpEn);
        }
    }

    /* display options */
    CMD_HELP_PRINT("options:\n");
    if(CMD_OPTION_EXIST(cmd)) {
        for(i = 0; i < cmd->optionsNum; i++) {
            opt = &cmd->options[i];
            CMD_HELP_FLUSH_BUF();
            CMD_HELP_PRINT("    ");
            if(opt->simpleName) CMD_HELP_PRINT("-%c", opt->simpleName);
            if(opt->simpleName && opt->name) CMD_HELP_PRINT(",");
            if(opt->name) CMD_HELP_PRINT("--%s", opt->name);
            CMD_HELP_PRINT(" ");
            cnt = snprintf(buf, CFAST_CMD_MAX_BUF_SIZE, "%-28s", buf);
            if(opt->isOptional) CMD_HELP_PRINT("[optional]");
            CMD_HELP_PRINT("%s\n", CMD_LANG_IS_CN(env)? opt->helpCn : opt->helpEn);
        }
    }

    CMD_HELP_PRINT("\n%s\n", CMD_LANG_IS_CN(env) ? cmd->helpCn : cmd->helpEn);
    env->outCallback(buf);
}

CFAST_VOID CFAST_CMD_SetSingleMode(CFAST_CMD_Env* env) {
    if(!env) return;
    env->isSingleMode = CFAST_TRUE;
}


CFAST_VOID CFAST_CMD_Execute(CFAST_CMD_Env* env, CFAST_SIZE argc, CFAST_CHAR** argv)
{
    CFAST_UINT i, j;
    CFAST_CHAR buf[CFAST_CMD_MAX_BUF_SIZE] = {0};
    CFAST_CMD_Command * cmd = CFAST_NULL;
    CFAST_CMD_Command * prevCmd = CFAST_NULL;
    CFAST_CMD_Command * subCmd = CFAST_NULL;
    CFAST_CHAR* str = CFAST_NULL;
    CFAST_INT optType = 0;
    if(argc < 1) {
        CMD_PRINT(env, "No command is given.");
        return;
    }

    if(!env->cmds || !env->cmdsNum) {
        CMD_PRINT_EX(env, buf, "No commands provided!\n");
        return;
    }

    /* 非单命令模式下，需要匹配主命令 */
    if(!env->isSingleMode) {
        for(i = 0; i < env->cmdsNum; i++) {
            if(strcmp(env->cmds[i].name, argv[i]) == 0) {
                CMD_PRINT(env, "[debug]command found cmd\n");
                cmd = env->cmds + i;
                break;
            }
        }
        if(!cmd) {
            CMD_PRINT_EX(env, buf, "Comamnd %s is not exist.\n", argv[0]);
            return;
        }
    } else {
        cmd = env->cmds;
    }

    CMD_PRINT_EX(env, buf, "->%s\n", cmd->name);

    /* 查找子命令 */
    prevCmd = cmd;
    for(i = 1; i < argc; i++) {
        if(argv[i][0] == '-') {
            subCmd = cmd;
            break;
        }
        subCmd = CFAST_NULL;
        if(!prevCmd->subCmd || !prevCmd->subCmd) {
            CMD_PRINT_EX(env, buf, "Error no subcommands provided, %s is not avalible.\n", argv[i]);
            break;
        }
        for(j = 0; j < prevCmd->subCmdNum; j++) {
            if(strcmp(prevCmd->subCmd[j].name, argv[i]) == 0) {
                subCmd = prevCmd->subCmd + j;
                break;
            }
        }
        if(!subCmd) {
            CMD_PRINT_EX(env, buf, "%s is not a subcommand of %s!\n", argv[i], prevCmd->name);
            return;
        }
        prevCmd = subCmd;
    }
    
    for(; i < argc; i++) {
        if(strstr(argv[i], "--")) {
            str = &argv[i][2];
            optType = 0;
        } else if(strstr(argv[i], "-")) {
            str = &argv[i][1];
            optType = 1;
        } else {
            CMD_PRINT_EX(env, buf, "parse error of %s!\n",  argv[i]);
            continue;
        }

        if(!prevCmd->options || !prevCmd->optionsNum) {
            CMD_PRINT_EX(env, buf, "%s is unknown.\n", argv[i]);
            break;
        }
        
        for(j = 0; j < prevCmd->optionsNum; j++) {
            if((optType == 0 && strcmp(str, prevCmd->options[j].name) == 0) || \
                (optType == 1 && str[0] == prevCmd->options[j].simpleName)) {
                prevCmd->callback(str);
                break;
            }
        }
        if(j >= prevCmd->optionsNum) {
            CMD_PRINT_EX(env, buf, "parse error of %s\n", argv[i]);
        }
    }
}
