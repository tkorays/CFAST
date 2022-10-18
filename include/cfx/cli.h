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
 * @defgroup CFX Command Line Interface
 * @ingroup CFX_CLI
 * @{
 * 
 * This module provides an interactive Command Line Interface(CLI).
 */

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
 * @brief input/output function.
 * 
 */
typedef struct cfx_cli_io cfx_cli_io_t;

/**
 * @brief command option portotype.
 * 
 * user can define their comamnd option in code, see struct cfx_cmd_opt_proto.
 * this struct will simplify the cli definition.
 */
typedef struct cfx_cmd_opt_proto cfx_cmd_opt_proto_t;

/**
 * @brief a command prototype.
 * 
 * user can define their comamnd in code, see struct cfx_cmd_proto.
 * this struct will simplify the cli definition.
 */
typedef struct cfx_cmd_proto cfx_cmd_proto_t;


typedef int(*cfx_cli_proc_fn)(const cfx_cli_t* cli, cf_void_t* sess, cfx_cli_opt_t* opts, cf_size_t argc, cf_char_t* argv[]);
typedef void(*cfx_cli_io_output_fn)(cf_void_t*, const cf_char_t*, ...);

/** see `cfx_cli_io_t` */
struct cfx_cli_io {
    cfx_cli_io_output_fn  output;   /** output function for CLI */
};

/** see `cfx_cli_opt_t` */
struct cfx_cli_opt {
    char                short_name; /** short name of this option */
    cf_string_t         long_name;  /** long name of this option */
    cfx_cli_opt_t*      next;       /** next option */
    cf_bool_t           required;   /** this option is required */
    cf_bool_t           flag;       /** a flag option will ouput true/false value */
    cf_bool_t           has_value;  /** has value */
    cf_string_t         value;      /** value of this option in string format */
};

/** see `cfx_cli_cmd_t` */
struct cfx_cli_cmd {
    //cfx_cli_cmd_t*      parent;     /** parent of this command */
    cfx_cli_cmd_t*      child;      /** child of this command */
    cfx_cli_cmd_t*      prev;       /** previous command in list */
    cfx_cli_cmd_t*      next;       /** next command in list */

    cf_string_t         name;       /** name of this command */
    cf_string_t         desc;       /** help text of this command */
    cfx_cli_opt_t*      opts;       /** command options, in list format */
    cfx_cli_proc_fn     proc;       /** process function, null means a command group */
};

/** see `cfx_cli_t` */
struct cfx_cli {
    cf_string_t         name;       /** name of this CLI */
    cf_string_t         desc;       /** help text for this CLI */
    cf_string_t         version;    /** CLI version */
    cfx_cli_cmd_t       root;       /** this root command */
    cfx_cli_io_t        io;         /** input and output function */
};


struct cfx_cmd_opt_proto {
    char                short_name;
    char*               long_name;
    char*               desc;
    int                 type;
    cf_bool_t           required;
    cf_bool_t           flag;
    char*               value;
};

struct cfx_cmd_proto {
    char*               name;
    char*               desc;
    char*               version;

    int                 opts_cnt;
    cfx_cmd_opt_proto_t*opts;

    int                 subcmds_cnt;
    cfx_cmd_proto_t*    subcmds;
};


cf_bool_t cfx_cli_init(cfx_cli_t* self, cfx_cli_io_t* io, const char* name, const char* desc, const char* ver);
cf_bool_t cfx_cli_init_from_proto(cfx_cli_t* self, cfx_cli_io_t* io, cfx_cmd_proto_t* proto);
void cfx_cli_deinit(cfx_cli_t* self);
cf_bool_t cfx_cli_input(cfx_cli_t* self, void* context, int argc, char* argv[]);
cf_bool_t cfx_cli_input_raw(cfx_cli_t* self, void* context, char* cmdstr);
cfx_cli_cmd_t* cfx_cli_root_cmd(cfx_cli_t* self);

cf_bool_t cfx_cli_cmd_init(cfx_cli_cmd_t* self, const char* name, const char* desc, cfx_cli_proc_fn fn);
void cfx_cli_cmd_deinit(cfx_cli_cmd_t* self);
cfx_cli_cmd_t* cfx_cli_cmd_sub(cfx_cli_cmd_t* self, char* name);
cfx_cli_opt_t* cfx_cli_cmd_opt(cfx_cli_cmd_t* self, char* opt);
cf_bool_t cfx_cli_cmd_add_sub(cfx_cli_cmd_t* self, cfx_cli_cmd_t* subcmd);
cf_bool_t cfx_cli_cmd_add_opt(cfx_cli_cmd_t* self, cfx_cli_opt_t* opt);

cf_bool_t cfx_cli_opt_init(cfx_cli_opt_t* self, char sn, const char* ln, const char* desc, cf_bool_t required, cf_bool_t flag, const char* def);
void cfx_cli_opt_deinit(cfx_cli_opt_t* self);
cf_bool_t cfx_cli_opt_exist_sn(cfx_cli_opt_t* self, char short_name);
cf_bool_t cfx_cli_opt_exist_ln(cfx_cli_opt_t* self, const char* long_name);
cf_bool_t cfx_cli_opt_has_val_sn(cfx_cli_opt_t* self, char short_name);
cf_bool_t cfx_cli_opt_has_val_ln(cfx_cli_opt_t* self, const char* long_name);

/** @} */

CF_DECLS_END

#endif /* __CFX_CLI_H__ */
