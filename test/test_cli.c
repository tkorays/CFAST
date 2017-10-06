#include "cfast/cli.h"
#include <stdio.h>

CFAST_VOID sub1Proc(CFAST_CLI_CmdOptResult* optInfo, CFAST_SIZE num) {
    CFAST_INT i;
    for(i = 0; i < num; i++) {
        switch(optInfo[i].id) {
            case 0:
                printf("get a\n");
                break;
            case 1:
                printf("get b:%s\n", optInfo[i].param);
                break;
            default:
                printf("no options match\n");
        }
    }
}

CFAST_CLI_CmdOptions opts[] = {
    {0, "a", "aaa", CFAST_FALSE, CFAST_FALSE, "opt a help", "a的帮助"},
    {1, "b", "bbb", CFAST_TRUE, CFAST_TRUE, "opt b help", "b的帮助"},
};

CFAST_CLI_Cmd subCmds[] = {
    {0, "sub1", CFAST_NULL, 0, opts, 2, sub1Proc, "help en", "中文帮助"},
};

CFAST_CLI_Cmd root = {
    0, "usercmd", subCmds, 1, opts, 2, CFAST_NULL, "user cmd help", "user cmd 中文"
};

int main(int argc, char** argv) {
    CFAST_CLI_Env* env = CFAST_CLI_CmdInit(CFAST_LANG_CN, CFAST_NULL, &root, CFAST_TRUE);
    //CFAST_CLI_CmdHelp(env, &subCmds, "usercmd");
    CFAST_CLI_CmdRun(env, argc, argv);
    CFAST_CLI_CmdUnInit(&env);
    return 0;
}

 /*
  1) 参数可选
  2) 重复参数设置，以第二次设置的为准
 */