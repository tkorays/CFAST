/****************************************************
 * Copyright (c) 2018, tkorays <tkorays@hotmail.com>
 * All rights reserved.
 * 
 * @file    assert.h
 * @brief   Assert of CFAST.
 * @version 1.0
 * @author  tkorays<tkorays@hotmail.com>
 * @date    2018-1-20
 ****************************************************/
#ifndef __CF_ASSERT_H__
#define __CF_ASSERT_H__

#include "cf/config.h"

#if CF_HAS_ASSERT
#  include <assert.h>
#else
#  define assert(expr)
#endif

#ifndef cf_assert
#  define cf_assert(expr) assert(expr)
#endif

#ifndef CF_ASSERT_RETURN
#  define CF_ASSERT_RETURN(expr, retval) \
    do {\
        if(!expr) cf_assert(expr);\
        return retval;\
    } while(0)
#endif

#ifndef CF_ASSERT_ON_FAIL
#  define CF_ASSERT_ON_FAIL(expr, exec_on_fail) \
    do {\
        cf_assert(expr);\
        if(!(expr)) exec_on_fail;\
    } while(0)
#endif

#endif /* __CF_ASSERT_H__ */
