/****************************************************
 * Copyright (c) 2018, tkorays <tkorays@hotmail.com>
 * All rights reserved.
 * 
 * @file    msg.h
 * @brief   A message component.
 * @version 1.0
 * @author  tkorays<tkorays@hotmail.com>
 * @date    2018-1-3
 ****************************************************/
#ifndef __CF_MSG_H__
#define __CF_MSG_H__

#include <cf/types.h>

CF_DECLS_BEGIN

#define CF_MSG_DATA_MAX_SIZE 1024
typedef cf_uint32_t cf_msg_uid_t;
typedef cf_uint32_t cf_msg_id_t;
typedef enum {
    cf_msg_prio_low = 0,
    cf_msg_prio_common,
    cf_msg_prio_high,
} cf_msg_prio_t;

typedef struct cf_msg {
    cf_msg_uid_t    from;
    cf_msg_uid_t    to;
    cf_msg_id_t     id;
    cf_msg_prio_t   priority;
    cf_uint32_t     param1;
    cf_uint32_t     param2;
    cf_uint32_t     param3;
    cf_uint16_t     size;
    cf_char_t       data[1];
} cf_msg_t;

typedef struct cf_msgq {
    cf_msg_t*   msg;
    cf_uint32_t size;
} cf_msgq_t;

cf_errno_t cf_msgq_init(cf_msgq_t que, cf_uint32_t quesize);
cf_errno_t cf_msgq_deinit(cf_msgq_t* que);
cf_errno_t cf_msgq_start(cf_msgq_t* que);
cf_errno_t cf_msgq_stop(cf_msgq_t* que);
cf_errno_t cf_msgq_send_sync(
    cf_msgq_t*      que, 
    cf_msg_uid_t    from,
    cf_msg_uid_t    to,
    cf_msg_id_t     id,
    cf_msg_prio_t   priority,
    cf_uint32_t     param1,
    cf_uint32_t     param2,
    cf_uint32_t     param3,
    cf_void_t*      data,
    cf_uint16_t     size
    );

cf_errno_t cf_msgq_send_async(
    cf_msgq_t*      que, 
    cf_msg_uid_t    from,
    cf_msg_uid_t    to,
    cf_msg_id_t     id,
    cf_msg_prio_t   priority,
    cf_uint32_t     param1,
    cf_uint32_t     param2,
    cf_uint32_t     param3,
    cf_void_t*      data,
    cf_uint16_t     size
    );

CF_DECLS_END

#endif /* __CF_MSG_H__ */
