/****************************************************
 * Copyright (c) 2018, tkorays <tkorays@hotmail.com>
 * All rights reserved.
 *
 * @file    version.h
 * @brief   version library.
 * @version 1.0
 * @author  tkorays<tkorays@hotmail.com>
 * @date    2023-2-14
 ****************************************************/
#ifndef __CF_VERSION_H__
#define __CF_VERSION_H__

#include "cf/types.h"

/**
 * @defgroup CF_VERSION version
 * a version tools
 * @{
 */

CF_DECLS_BEGIN

/**
 * version struct
 */
typedef struct {
    int     major;  /** major version */
    int     minor;  /** minor version */
    int     patch;  /** patch version */
    int     pretag; /** prelease tag,a/b are valid */
    int     prenum; /** prelease number */
} cf_version_t;


/**
 * parse version from string
 * @param   v   output version
 * @param   t   version string
 * @return true if parsing is successful
 */
cf_bool_t cf_version_from(cf_version_t* v, const char* t);

/**
 * compare two versions
 * @param   v1  version1
 * @param   v2  version2
 * @return 1 if v1 > v2; -1 if v1 < v2; 0 if v1 == v2
 */
int cf_version_compare(const cf_version_t* v1, const cf_version_t* v2);

/**
 * check if two versions are equal
 * @param   v1  version1
 * @param   v2  version2
 * @return  true if v1 == v2
 */
cf_bool_t cf_version_eq(const cf_version_t* v1, const cf_version_t* v2);

/**
 * check if v1 is greater than v2
 * @param   v1  version1
 * @param   v2  version2
 * @return  true if v1 > v2
 */
cf_bool_t cf_version_gt(const cf_version_t* v1, const cf_version_t* v2);

/**
 * check if v1 is less than v2
 * @param   v1  version1
 * @param   v2  version2
 * @return  true if v1 < v2
 */
cf_bool_t cf_version_lt(const cf_version_t* v1, const cf_version_t* v2);

/**
 * check if v1 is greater than or equal to v2
 * @param   v1  version1
 * @param   v2  version2
 * @return  true if v1 >= v2
 */
cf_bool_t cf_version_ge(const cf_version_t* v1, const cf_version_t* v2);

/**
 * check if v1 is less than or equal to v2
 * @param   v1  version1
 * @param   v2  version2
 * @return  true if v1 <= v2
 */
cf_bool_t cf_version_le(const cf_version_t* v1, const cf_version_t* v2);

CF_DECLS_END

/** @} */

#endif /* __CF_VERSION_H__ */

