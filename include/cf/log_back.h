#ifndef __CFAST_LOG_H__
#define __CFAST_LOG_H__

#include "cfast_def.h"

typedef enum {
    CFAST_LOG_LEVEL_DEBUG = 0x0001,
    CFAST_LOG_LEVEL_INFO = 0x0002,
    CFAST_LOG_LEVEL_WARN = 0x0004,
    CFAST_LOG_LEVEL_ERROR = 0x0008,
    CFAST_LOG_LEVEL_FATAL = 0x0010
} CFAST_LOG_LEVEL;

#define CFAST_LOG_LEVEL_OFF 0
#define CFAST_LOG_LEVEL_ALL 0xFFFF

/* [2017-01-02-12:32:00:900][P(1234)|T(2333)][demo.c:2233][FuncABC] */
#define CFAST_LOG_FORMAT "[%s][P(%d)|T(%d)][%s:%d][%s]"

#define CFAST_LOG_Debug(cid, tid, fmtStr, ...)
#define CFAST_LOG_Info(cid, tid, fmtStr, ...)
#define CFAST_LOG_Warn(cid, tid, fmtStr, ...)
#define CFAST_LOG_Error(cid, tid, fmtStr, ...)
#define CFAST_LOG_Fatal(cid, tid, fmtStr, ...)

typedef struct {
    CFAST_HANDLE fileHandle;
    CFAST_HANDLE lock;

} CFAST_LOG_Logger;

CFAST_RET CFAST_LOG_Init();

CFAST_RET CFAST_LOG_Start(CFAST_CHAR* path, CFAST_LOG_Logger* logger);

CFAST_VOID CFAST_LOG_Log(CFAST_LOG_LEVEL level, CFAST_INT cid, CFAST_INT tid, const CFAST_CHAR* fmtStr, ...);

#endif /* __CFAST_LOG_H__ */
