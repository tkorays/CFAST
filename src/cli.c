#include "cfast/cli.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#define CLI_ENV_PRINT(env, fmtStr, ...) \
    do { \
        if((env)->printFunc) (env)->printFunc(fmtStr, ##__VA_ARGS__); \
    } while(0)
#define CLI_CMD_MATCH_SHORT(s) (strlen((s)) > 1 && (s)[0] == '-' && (s)[1] != '-')
#define CLI_CMD_MATCH_LONG(s) (strlen((s)) > 2 && (s)[0] == '-' && (s)[1] == '-')
#define CLI_CMD_OPT_GET_HELP(p, lang) (CFAST_LANG_IS_EN((lang)) ? (p)->helpEn : (p)->helpLocal)
#define CLI_CMD_HAS_SHORT(p) ((p)->shortName && (p)->shortName[0])
#define CLI_CMD_HAS_LONG(p) ((p)->longName && (p)->longName[0])
#define CLI_CMD_HAS_SHORT_LONG(p) (CLI_CMD_HAS_SHORT(p) && CLI_CMD_HAS_LONG(p))

CFAST_INT CLI_DefaultPrintFunc(CFAST_CHAR* fmtStr, ...) {
    CFAST_INT cnt;
    va_list args;
    va_start(args, fmtStr);
    cnt = vprintf(fmtStr, args);
    va_end(args);
    return cnt;
}

CFAST_CLI_Env* CFAST_CLI_CmdInit(CFAST_LANG lang, CFAST_CLI_PrintFunc func, CFAST_CLI_Cmd* cmd, CFAST_BOOL skipRoot) {
    CFAST_CLI_Env* env = CFAST_NULL;
    if(lang >= CFAST_LANG_OTHER || cmd == CFAST_NULL) return CFAST_NULL;
    env = (CFAST_CLI_Env*)malloc(sizeof(CFAST_CLI_Env));
    if(!env) return CFAST_NULL;
    env->lang = lang;
    env->printFunc = func ? func : CLI_DefaultPrintFunc;
    env->cmd = cmd;
    env->skipRoot = skipRoot;
    return env;
}

CFAST_VOID CFAST_CLI_CmdUnInit(CFAST_CLI_Env** env) {
    if(!env || !*env) return ;
    free(*env);
    *env = CFAST_NULL;
}

CFAST_VOID CFAST_CLI_CmdHelp(CFAST_CLI_Env* env, CFAST_CLI_Cmd* cmd, CFAST_CHAR* rootCmd) {
    CFAST_CHAR buf[CFAST_CLI_MAX_BUF_SIZE] = {0};
    CFAST_INT wrtCnt = 0;
    CFAST_INT i;
    if(!env || !cmd) return ;

    /* 命令详情 */
    CLI_ENV_PRINT(env, "Overview: %s\n\n", CLI_CMD_OPT_GET_HELP(cmd, env->lang));

    /* 显示该命令使用详情 */
    CLI_ENV_PRINT(env, "Usage:\n  %s%s <command> [options] ... \n\n",
        rootCmd ? rootCmd : "", 
        rootCmd ? " " : "");

    /* 显示该命令的所有子命令 */
    CLI_ENV_PRINT(env, "Commands:\n");
    for(i = 0; (i < cmd->subCmdNum) && cmd->subCmd; i++) {
        CLI_ENV_PRINT(env, "  %-28s %s\n", cmd->subCmd[i].name, CLI_CMD_OPT_GET_HELP(cmd->subCmd + i, env->lang));
    }
    if(cmd->subCmdNum <= 0) CLI_ENV_PRINT(env, "  None\n");

    /* 显示该命令的配置 */
    CLI_ENV_PRINT(env, "\nGeneral Options:\n");
    for(i = 0; (i < cmd->optionNum) && cmd->option; i++) {
        if(CLI_CMD_HAS_SHORT_LONG(cmd->option + i)) {
            wrtCnt = snprintf(buf, CFAST_CLI_MAX_BUF_SIZE, "-%s,--%s", cmd->option[i].shortName, cmd->option[i].longName);
        } else if(CLI_CMD_HAS_SHORT(cmd->option + i)) {
            wrtCnt = snprintf(buf, CFAST_CLI_MAX_BUF_SIZE, "-%s", cmd->option[i].shortName);
        } else if(CLI_CMD_HAS_LONG(cmd->option + i)) {
            wrtCnt = snprintf(buf, CFAST_CLI_MAX_BUF_SIZE, "--%s", cmd->option[i].longName);
        }
        if(cmd->option[i].needParam) {
            wrtCnt = snprintf(buf + wrtCnt, CFAST_CLI_MAX_BUF_SIZE - wrtCnt, "%s", " <arg>");
        }
        CLI_ENV_PRINT(env, "  %-28s %s\n", buf, CLI_CMD_OPT_GET_HELP(cmd->option + i, env->lang));
    }
    if(cmd->optionNum <= 0) CLI_ENV_PRINT(env, "  None\n");
    CLI_ENV_PRINT(env, "\n");
}

CFAST_VOID CFAST_CLI_CmdRun(CFAST_CLI_Env* env, CFAST_SIZE argc, CFAST_CHAR** argv) {
    CFAST_INT i, j;
    CFAST_BOOL needParam = CFAST_FALSE;
    CFAST_CLI_Cmd* curCmd = CFAST_NULL;
    CFAST_CLI_CmdOptResult result[CFAST_CLI_MAX_OPT_NUM] = {0};
    CFAST_SIZE optCnt = 0;

    CFAST_CHAR buf[CFAST_CLI_MAX_BUF_SIZE] = {0};
    CFAST_SIZE wrtCnt = 0;
    if(!env) return;

    if(argc < 1 || !argv) { CLI_ENV_PRINT(env, "Error: no arguments!\n"); return ; }
    
    /* 匹配根命令是否匹配，一般需要设置为true，跳过第一个命令的检查 */
    if(!env->skipRoot && strcmp(env->cmd->name, argv[0]) != 0) {
        CLI_ENV_PRINT(env, "Error: root command not match!\n");
        return ;
    }

    curCmd = env->cmd;
    wrtCnt = snprintf(buf, CFAST_CLI_MAX_BUF_SIZE, "%s", curCmd->name);
    for(i = 1; i < argc; i++) {
        if(strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0) {
            CFAST_CLI_CmdHelp(env, curCmd, buf);
            break;
        }

        if(CLI_CMD_MATCH_SHORT(argv[i])) {
            if(needParam) {
                CLI_ENV_PRINT(env, "Error: param required!\n");
                return;
            }
            for(j = 0; j < curCmd->optionNum && (strcmp(curCmd->option[j].shortName, argv[i]+1) != 0); j++) ;
            if(j < curCmd->optionNum) {
                result[optCnt++].id = curCmd->option[j].id;
                needParam = curCmd->option[j].needParam;
            } else {
                CLI_ENV_PRINT(env, "Warning: unrecognized option %s!\n", argv[i]);
            }
        } else if(CLI_CMD_MATCH_LONG(argv[i])) {
            if(needParam) {
                CLI_ENV_PRINT(env, "Error: param required!\n");
                return;
            }
            for(j = 0; j < curCmd->optionNum && (strcmp(curCmd->option[j].longName, argv[i]+2) != 0); j++) ;
            if(j < curCmd->optionNum) {
                result[optCnt++].id = curCmd->option[j].id;
                needParam = curCmd->option[j].needParam;
            } else {
                CLI_ENV_PRINT(env, "Warning: unrecognized option %s!\n", argv[i]);
            }
        } else if(needParam){
            result[optCnt - 1].param = argv[i];
            needParam = CFAST_FALSE;
        } else if(optCnt > 0) {
            CLI_ENV_PRINT(env, "Error: command error!\n");
        } else {
            /* 查找子命令 */
            for(j = 0; j < curCmd->subCmdNum && (strcmp(curCmd->subCmd[j].name, argv[i]) != 0); j++); 
            if(j < curCmd->subCmdNum) {
                curCmd = curCmd->subCmd + j;
                wrtCnt = snprintf(buf + wrtCnt, CFAST_CLI_MAX_BUF_SIZE - wrtCnt, " %s", curCmd->name);
            } else {
                CLI_ENV_PRINT(env, "Error: unrecognized subcommand %s!\n", argv[i]);
                return ;
            }
        }
    }
    if(needParam) {
        CLI_ENV_PRINT(env, "Error: param required!\n");
        return;
    }

    /* 检查必需的字段是否存在 */
    for(i = 0; i < curCmd->optionNum; i++) {
        if(curCmd->option[i].essential) {
            for(j = 0; j < optCnt && (result[j].id != curCmd->option[i].id); j++);
            if(j >= optCnt) {
                if(CLI_CMD_HAS_SHORT_LONG(curCmd->option + i)) {
                    CLI_ENV_PRINT(env, "Error: option -%s,--%s is required!\n", curCmd->option[i].shortName, curCmd->option[i].longName);
                } else if(CLI_CMD_HAS_SHORT(curCmd->option + i)) {
                    CLI_ENV_PRINT(env, "Error: option -%s is required!\n", curCmd->option[i].shortName);
                } else if(CLI_CMD_HAS_LONG(curCmd->option + i)) {
                    CLI_ENV_PRINT(env, "Error: option --%s is required!\n", curCmd->option[i].longName);
                }
                return;
            }
        }
    }
    if(curCmd->procFunc) {
        curCmd->procFunc(result, optCnt);
    }
}

