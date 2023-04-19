/****************************************************
 * Copyright (c) 2022, tkorays <tkorays@hotmail.com>
 * All rights reserved.
 * 
 * @file    cli.h
 * @brief   Command line interface.
 * @version 1.0
 * @author  tkorays<tkorays@hotmail.com>
 * @date    2022-10-14
 ****************************************************/
#ifndef __CFX_CLI_H__
#define __CFX_CLI_H__

#include "cf/types.h"
#include "cf/string.h"


CF_DECLS_BEGIN

/**
 * @defgroup CFX_CLI CLIx
 * This module provides an interactive Command Line Interface(CLI).
 *
 * @{
 */

#define CFX_CLI_OPT_NONE     (0)
#define CFX_CLI_OPT_REQ      (1)
#define CFX_CLI_OPT_IS_FLAG  (1 << 1)

/**
 * @brief a cfx_cli_t instance will manage all CLI commands.
 * 
 */
typedef struct cfx_cli cfx_cli_t;

/**
 * @brief a command or command group.
 * 
 * command with subcommands is a command group and will not process in itself.
 */
typedef struct cfx_cli_cmd cfx_cli_cmd_t;

/**
 * @brief options like -h/--help.
 * 
 * an option could have either short name or long name.
 */
typedef struct cfx_cli_opt cfx_cli_opt_t;


/**
 * @brief command process function
 * 
 */
typedef int(*cfx_cli_proc_fn)(cf_void_t* sess,
                               const cfx_cli_opt_t* opts,
                               cf_size_t argc,
                               cf_char_t* argv[]);


/**
 * @brief create a CLI object with name, description and version
 * 
 * @param name  name of this CLI 
 * @param desc  decription 
 * @param ver   version of this CLI 
 * @return cfx_cli_t* a new CLI object
 */
cfx_cli_t* cfx_cli_new(const char* name, const char* desc, const char* ver);

/**
 * @brief destroy CLI object
 * 
 * @param self 
 */
void cfx_cli_delete(cfx_cli_t* self);

/**
 * @brief input args and execute commands
 * 
 * @param self      CLI object
 * @param context   context of io, NULL if none
 * @param argc      arguments count
 * @param argv      arguments vector
 * @return cf_bool_t    success or not 
 */
cf_bool_t cfx_cli_input(cfx_cli_t* self, void* context, int argc, char* argv[]);

/**
 * @brief get the root command
 * 
 * @param self CLI object
 * @return cfx_cli_cmd_t* the root command
 */
cfx_cli_cmd_t* cfx_cli_get_root_cmd(cfx_cli_t* self);

/**
 * @brief create a new command
 * 
 * @param name name of command
 * @param desc description of this command
 * @param fn command processor
 * @return cfx_cli_cmd_t*   the created command
 */
cfx_cli_cmd_t* cfx_cli_cmd_new(const char* name, const char* desc, cfx_cli_proc_fn fn);

/**
 * @brief destroy a command
 * 
 * @param cmd the command to be destroyed
 * @return cf_void_t 
 */
cf_void_t cfx_cli_cmd_delete(cfx_cli_cmd_t* cmd);

/**
 * @brief add a sub command to a parent command
 * 
 * @param parent parent command
 * @param subcmd sub command
 * @return cf_void_t 
 */
cf_void_t cfx_cli_cmd_add_sub(cfx_cli_cmd_t* parent, cfx_cli_cmd_t* subcmd);

/**
 * @brief add a new option to a command
 * 
 * @param cmd   command the option belongs to 
 * @param sn    short name of the option 
 * @param ln    long name of the option
 * @param desc  description of the option
 * @param flags flags of the option
 * @param def   default value, default NULL
 * @return cfx_cli_opt_t* new option
 */
cfx_cli_opt_t* cfx_cli_opt_add(cfx_cli_cmd_t* cmd, char sn, const char* ln, const char* desc, cf_uint32_t flags, const char* def);

/**
 * @brief get the option by short name or long name
 * 
 * @param opts  entry of options
 * @param opt   option name, short or long style
 * @return cf_char_t* option value, return NULL if option is not found
 */

cf_char_t* cfx_cli_opt_val(const cfx_cli_opt_t* opts, char* name);

/** @} */

CF_DECLS_END

#endif /* __CFX_CLI_H__ */
