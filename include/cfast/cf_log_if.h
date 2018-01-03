/**********************************************
 * Copyright (c) 2018, tkorays
 * All rights reserved.
 * 
 * @file    cf_log_if.h
 * @brief   CFAST的日志模块对外接口
 * @version 1.0
 * @author  tkorays<tkorays@hotmail.com>
 * @date    2018-1-3
 *********************************************/
#ifndef __CF_LOG_IF_H__
#define __CF_LOG_IF_H__

typedef enum {
    CF_LOG_LEVEL_DEBUG  = 0x0001,
    CF_LOG_LEVEL_INFO   = 0x0002,
    CF_LOG_LEVEL_WARN   = 0x0004,
    CF_LOG_LEVEL_ERROR  = 0x0008,
    CF_LOG_LEVEL_FATAL  = 0x0010
} CFAST_LOG_LEVEL;

#endif /* __CF_LOG_IF_H__ */
