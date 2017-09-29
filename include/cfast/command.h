#ifndef __CFAST_COMMAND_H__
#define __CFAST_COMMAND_H__

#include "cfast_def.h"

#define CFAST_CMD_MAX_NAME_SIZE 32
#define CFAST_CMD_MAX_OPTIONS   32
#define CFAST_CMD_MAX_ARGS      32

#define CFAST_CMD_MAX_HELP_SIZE 256
#define CFAST_CMD_MAX_BUF_SIZE  1024

typedef CFAST_VOID(*CFAST_CMD_OptionCallback)(CFAST_CHAR* args);
typedef CFAST_VOID(*CFAST_CMD_OutputCallback)(CFAST_CHAR* out);

typedef enum {
    CFAST_LANG_EN,
    CFAST_LANG_CN
} CFAST_LANGUAGE_TYPE;

typedef struct {
    CFAST_CHAR* name;                  /* command name */
    CFAST_CHAR simpleName;              /* simple name */
    CFAST_BOOL isOptional;              /* is optional */
    CFAST_CHAR* helpEn;                /* english help text */
    CFAST_CHAR* helpCn;                /* chinese help text */
    CFAST_CMD_OptionCallback callback;  /* option callback function */
} CFAST_CMD_Option;

typedef struct _Command {
    CFAST_CHAR* name;     
    CFAST_CHAR* helpEn;  
    CFAST_CHAR* helpCn;
    CFAST_BOOL ignoreCase;
    CFAST_CMD_Option* options;
    CFAST_SIZE optionsNum;
    struct _Command* subCmd;
    CFAST_SIZE subCmdNum;
    CFAST_CMD_OptionCallback callback;
} CFAST_CMD_Command;

typedef struct {
    CFAST_CMD_Command* cmds;
    CFAST_SIZE cmdsNum;
    CFAST_LANGUAGE_TYPE lang;
    CFAST_BOOL isSingleMode;
    // CFAST_CHAR* version;
    CFAST_CMD_OutputCallback outCallback;
} CFAST_CMD_Env;


CFAST_RET CFAST_CMD_Init(CFAST_CMD_Env* env, CFAST_CMD_Command* cmdDef, CFAST_SIZE cmdNum);
CFAST_RET CFAST_CMD_UnInit(CFAST_CMD_Env* env);
CFAST_VOID CFAST_CMD_SetOutFunc(CFAST_CMD_Env* env, CFAST_CMD_OutputCallback outCallback);
CFAST_VOID CFAST_CMD_PrintHelp(CFAST_CMD_Env* env, CFAST_CMD_Command* cmd);
CFAST_VOID CFAST_CMD_SetSingleMode(CFAST_CMD_Env* env);
CFAST_VOID CFAST_CMD_Execute(CFAST_CMD_Env* env, CFAST_SIZE argc, CFAST_CHAR** argv);


#endif /* __CFAST_COMMAND_H__ */
