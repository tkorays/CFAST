#include <stdio.h>
#include "include/cfast/command.h"


void abcProc(CFAST_CHAR* args) {
    printf("process options: %s\n", args);
}

CFAST_CMD_Option opts[] = {
    {"enable-fuck", '\0', CFAST_FALSE, "help text for abc", "abc的帮助文档", abcProc},
    {"config-dir", 'E', CFAST_TRUE, "help text for efg", "efg的帮助文档", CFAST_NULL}
};

CFAST_CMD_Command subCmds[] = {
    {
    "WTF",
    "wtf help en",
    "wtf 中文帮助文档",
    CFAST_TRUE,
    opts,
    CFAST_SIZEOF_ARRAY(opts, CFAST_CMD_Option),
    CFAST_NULL,
    0,
    abcProc
}
};

CFAST_CMD_Command cmd[] = {
    {
        "foo",
        "foo help text",
        "foo的帮助，我要写的很长，假装我这里有很多帮助一样。哈哈哈，就是这么任性。",
        CFAST_TRUE,
        opts,
        CFAST_SIZEOF_ARRAY(opts, CFAST_CMD_Option),
        subCmds,
        1,
        abcProc
    },
};



CFAST_VOID Print2Console(CFAST_CHAR* output) {
    printf("%s", output);
}

int main(int argc, char** argv) {
    CFAST_CMD_Env env;
    CFAST_CHAR* a = CFAST_NULL;
    (CFAST_VOID)CFAST_CMD_Init(&env, cmd, CFAST_SIZEOF_ARRAY(cmd, CFAST_CMD_Command));
    CFAST_CMD_SetOutFunc(&env, Print2Console);
    CFAST_CMD_SetSingleMode(&env);
    CFAST_CMD_PrintHelp(&env, env.cmds);



    CFAST_CMD_Execute(&env, argc, argv);
    return 0;
}