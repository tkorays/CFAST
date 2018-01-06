/****************************************************
 * Copyright (c) 2018, tkorays <tkorays@hotmail.com>
 * All rights reserved.
 * 
 * @file    cf_cli_if.h
 * @brief   Command line interface.
 * @version 1.0
 * @author  tkorays<tkorays@hotmail.com>
 * @date    2018-1-6
 ****************************************************/
#ifndef __CF_CLI_IF_H__
#define __CF_CLI_IF_H__

#include "cf_def.h"
#include "cf_list_if.h"

typedef struct cf_cli_cmd_cxt cf_cli_cmd_cxt_t;
typedef cf_void_t(*cf_cli_pfn_proc_cmd)(cf_cli_cmd_cxt_t* cxt, cf_char_t* s);

/**
 * A command option.
 */
typedef struct {
    cf_id_t     id;         /** command id */
    cf_char_t*  short_name; /** The short name of a command option. */
    cf_char_t*  long_name;  /** The long name of a command option. */
    cf_bool_t   optional;   /** Is this option optional? */
    cf_char_t   help;       /** Help. */
} cf_cli_cmdopt_t;

typedef struct {
    cf_id_t                 id;
    cf_char_t*              name;
    cf_list_t*              options;
    cf_list_t*              subcmds;
    cf_char_t*              help;
    cf_cli_pfn_proc_cmd     proc_cmd;
} cf_cli_cmd_t;

/**
 * Context for a command.
 */
typedef struct cf_cli_cmd_cxt {
    cf_cli_cmd_t*   cmd;
    cf_void_t*      data;
} cf_cli_cmd_cxt_t;

#endif /* __CF_CLI_IF_H__ */
