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


#define CF_CLI_INVALID_ID -1

typedef struct cf_cli_cmd_cxt cf_cli_cmd_cxt_t;
typedef cf_errno_t(*cf_cli_pfn_proc_cmd)(cf_cli_cmd_cxt_t* cxt, cf_char_t* s);
typedef cf_void_t(*cf_cli_pfn_input)(cf_char_t* buf, cf_size_t bufsize);
typedef cf_void_t(*cf_cli_pfn_output)(cf_char_t* buf);

/**
 * A command option.
 */
typedef struct {
    cf_id_t     id;         /** command id */
    cf_char_t*  short_name; /** The short name of a command option. */
    cf_char_t*  long_name;  /** The long name of a command option. */
    cf_bool_t   optional;   /** Is this option optional? */
    cf_char_t*  help;       /** Help. */
} cf_cli_cmdopt_t;

/**
 * A command.
 */
typedef struct cf_cli_cmd_s {
    cf_id_t                 id;
    cf_char_t*              name;
    cf_size_t               option_cnt;
    cf_cli_cmdopt_t*        option;
    cf_size_t               subcmd_cnt;
    struct cf_cli_cmd_s*    subcmd;
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

typedef struct cf_cli {
    cf_cli_cmd_t*       root;   /* one root */
    cf_cli_pfn_input    input;
    cf_cli_pfn_output   output;
    cf_char_t           inbuf[1024];
} cf_cli_t;

cf_cli_t*   cf_cli_init();
cf_void_t   cf_cli_deinit(cf_cli_t* cli);
cf_errno_t  cf_cli_set_io_func(cf_cli_t* cli, cf_cli_pfn_input input, cf_cli_pfn_output output);
cf_errno_t  cf_cli_install_all_cmds(cf_cli_t* cli, cf_cli_cmd_t* root);
cf_errno_t  cf_cli_run_line(cf_cli_t* cli, cf_char_t* line);
cf_errno_t  cf_cli_run(cf_cli_t* cli);



#endif /* __CF_CLI_IF_H__ */
