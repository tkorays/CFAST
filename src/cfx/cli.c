#include "cfx/cli.h"
#include "cf/str.h"
#include <stdio.h>
#include <stdarg.h>

#define CFX_DEF_CLI_LICENSE "CFAST CLI v1.0, Author(tkorays), All right reserved!\n"



cfx_cmd_opt_proto_t opts_for_root[] = {
    {
        .short_name = 'a',
        .long_name  = "abc",
        .desc       = "help for abc cmd",
        .type       = 0,
        .required   = CF_TRUE,
        .flag       = CF_FALSE,
        .value      = "",
    }
};

cfx_cmd_proto_t cmd_root = {
    .name = "wtf",
    .desc = "help for wtf",
    .version = "1.0.0",

    .opts_cnt   = CF_ARRAY_SIZE(opts_for_root),
    .opts       = &opts_for_root[0],
    .subcmds_cnt  = 0,
    .subcmds = CF_NULL_PTR,
};

void _cfx_cli_io_output_fn(cf_void_t* ctx, const cf_char_t* fmt, ...) {
    va_list args;
    cf_char_t buf[1024] = {0};
    va_start(args, fmt);
    vsprintf_s(buf, sizeof(buf), fmt, args);
    /* vsprintf(buf, fmt, args); */
    va_end(args);
    printf(buf);
}

cfx_cli_io_t _cfx_cli_default_io_ = {
    .output = _cfx_cli_io_output_fn
};

cf_bool_t cfx_cli_init(cfx_cli_t* self, cfx_cli_io_t* io, const char* name, const char* desc, const char* ver) {
    cf_membzero(self, sizeof(cfx_cli_t));

    cf_string_init(&self->name, cf_strlen(name), name);
    cf_string_init(&self->desc, cf_strlen(desc), desc);
    cf_string_init(&self->version, cf_strlen(ver), ver);

    cf_string_init_with_ref(&self->root.name, cf_string_ptr(&self->name), cf_string_len(&self->name));
    cf_string_init_with_ref(&self->root.desc, cf_string_ptr(&self->desc), cf_string_len(&self->desc));

    if (!io) {
        self->io    = _cfx_cli_default_io_;
    } else {
        self->io    = *io;
    }
    return CF_TRUE;
}

cf_bool_t cfx_cli_init_from_proto(cfx_cli_t* self, cfx_cli_io_t* io, cfx_cmd_proto_t* proto) {
    return CF_TRUE;
}

void cfx_cli_deinit(cfx_cli_t* self) {
    cf_string_deinit(&self->name);
    cf_string_deinit(&self->desc);
    cf_string_deinit(&self->version);
}

#define CLI_STATE_CMD 0
#define CLI_STATE_OPT 1
#define CLI_STATE_ARG 2
#define CLI_OPT_SUBSTATE_WAITING_FOR_OPT 0
#define CLI_OPT_SUBSTATE_WAITING_FOR_VAL 1

cf_bool_t cfx_cli_input(cfx_cli_t* self, void* context, int argc, char* argv[]) {
    cfx_cli_cmd_t* parent = &self->root;
    cfx_cli_cmd_t* cmd = parent;
    cfx_cli_opt_t* opt = CF_NULL_PTR;
    int i;
    int state = CLI_STATE_CMD;
    int substate = CLI_OPT_SUBSTATE_WAITING_FOR_OPT;
    cf_bool_t is_opt = CF_FALSE;
    cf_bool_t opt_is_long_name = CF_FALSE;
    
    /* the first one should be the cli name */
    for (i = 1; i < argc; i++) {
        is_opt = cf_str_startwith(argv[i], "-");
        opt_is_long_name = cf_str_startwith(argv[i], "--");

        if (state == CLI_STATE_CMD) {
            if (!is_opt) {
                cmd = cfx_cli_cmd_sub(cmd, argv[i]);
                if (!cmd) {
                    /* leave command state and enter argument state */
                    cmd = parent;
                    state = CLI_STATE_ARG;
                    substate = CLI_OPT_SUBSTATE_WAITING_FOR_OPT;
                    break;
                } else {
                    parent = cmd;
                }
            } else {
                /* from command state to option state */
                state = CLI_STATE_OPT;
                substate = CLI_OPT_SUBSTATE_WAITING_FOR_VAL;
                // TODO: handle option
            }
        } else if (state == CLI_STATE_OPT) {
            if (!cmd) {
                self->io.output(CF_NULL_PTR, "command not found!");
                return CF_FALSE;
            }

            if (substate == CLI_OPT_SUBSTATE_WAITING_FOR_OPT && is_opt) {
                /* this is an option we should wait for value if option requires value */
                if (opt_is_long_name) {
                    if (cf_strlen(argv[i]) <= 2) {
                        self->io.output(CF_NULL_PTR, "option format is not correct: \"%s\"", argv[i]);
                        return CF_FALSE;
                    }
                    opt = cfx_cli_cmd_opt(cmd, &argv[i][2]);
                } else {
                    if (cf_strlen(argv[i]) <= 1) {
                        self->io.output(CF_NULL_PTR, "option format is not correct: \"%s\"", argv[i]);
                        return CF_FALSE;
                    }
                    opt = cfx_cli_cmd_opt(cmd, &argv[i][1]);
                }
                if (!opt) {
                    self->io.output(CF_NULL_PTR, "no option named %s", argv[i]);
                    return CF_FALSE;
                }
                substate = CLI_OPT_SUBSTATE_WAITING_FOR_VAL;
            } else if (substate == CLI_OPT_SUBSTATE_WAITING_FOR_VAL && is_opt) {
                // TODO: 
                // 连遇到两个opt，前一个可能有参数，可能没有参数，需要根据情况决定是否异常


                if (opt_is_long_name) {
                    if (cf_strlen(argv[i]) <= 2) {
                        self->io.output(CF_NULL_PTR, "option format is not correct: \"%s\"", argv[i]);
                        return CF_FALSE;
                    }
                    opt = cfx_cli_cmd_opt(cmd, &argv[i][2]);
                }
                else {
                    if (cf_strlen(argv[i]) <= 1) {
                        self->io.output(CF_NULL_PTR, "option format is not correct: \"%s\"", argv[i]);
                        return CF_FALSE;
                    }
                    opt = cfx_cli_cmd_opt(cmd, &argv[i][1]);
                }
                if (!opt) {
                    self->io.output(CF_NULL_PTR, "no option named %s", argv[i]);
                    return CF_FALSE;
                }
                substate = CLI_OPT_SUBSTATE_WAITING_FOR_VAL;
            } else if (substate == CLI_OPT_SUBSTATE_WAITING_FOR_OPT && !is_opt) {
                /* no pending options, enter argument state */
                state = CLI_STATE_ARG;
                substate = CLI_OPT_SUBSTATE_WAITING_FOR_OPT;
                break;
            } else if (substate == CLI_OPT_SUBSTATE_WAITING_FOR_VAL && !is_opt) {
                //TODO: fetch value
                opt->value;
                substate = CLI_OPT_SUBSTATE_WAITING_FOR_OPT;
            }
        }
    }

    if (!cmd) {
        self->io.output(CF_NULL_PTR, "command not found!");
        return CF_FALSE;
    }
    if (!cmd->proc) {
        self->io.output(CF_NULL_PTR, "command \"%s\" is a GROUP! we will say help later.", cf_string_ptr(&cmd->name));
        return CF_FALSE;
    }

    if (cmd && cmd->proc) {
        cmd->proc(self, CF_NULL_PTR, cmd->opts, argc - i, &argv[i]);
    }
    return CF_TRUE;
}

cf_bool_t cfx_cli_input_raw(cfx_cli_t* self, void* context, char* cmdstr) {
    return CF_FALSE;
}

cfx_cli_cmd_t* cfx_cli_root_cmd(cfx_cli_t* self) {
    return &self->root;
}


cf_bool_t cfx_cli_cmd_init(cfx_cli_cmd_t* self, const char* name, const char* desc, cfx_cli_proc_fn fn) {
    if (!name || !desc || !fn) {
        return CF_FALSE;
    }

    cf_membzero(self, sizeof(cfx_cli_cmd_t));
    cf_string_init(&self->name, cf_strlen(name), name);
    cf_string_init(&self->desc, cf_strlen(desc), name);
    self->proc = fn;

    return CF_TRUE;
}

void cfx_cli_cmd_deinit(cfx_cli_cmd_t* self) {
    cfx_cli_cmd_t* cmd, *cmd_next;
    cfx_cli_opt_t* opt, *opt_next;
    /* deinit child */
    cfx_cli_cmd_deinit(self->child);
    self->child = CF_NULL_PTR;

    
    /* deinit sublings */
    cmd = self->next;
    while (cmd) {
        cmd_next = cmd->next;
        cfx_cli_cmd_deinit(cmd);
        cmd = cmd_next;
    }

    /* deinit self */
    cf_string_deinit(&self->name);
    cf_string_deinit(&self->desc);

    opt = self->opts;
    while (opt) {
        opt_next = opt->next;
        cf_string_deinit(&opt->long_name);
        cf_string_deinit(&opt->value);
        cf_free(opt);
        opt = opt_next;
    }

    self->next = CF_NULL_PTR;
    self->prev = CF_NULL_PTR;
}

cfx_cli_cmd_t* cfx_cli_cmd_sub(cfx_cli_cmd_t* self, char* name) {
    cfx_cli_cmd_t* child = self->child;
    if (!name) {
        return CF_NULL_PTR;
    }
    
    while (child) {
        if (cf_strcmp(cf_string_ptr(&child->name), name) == 0) {
            return child;
        }
        self = child->next;
    }
    return CF_NULL_PTR;
}

cfx_cli_opt_t* cfx_cli_cmd_opt(cfx_cli_cmd_t* self, char* opt) {
    cfx_cli_opt_t* opts;
    if (!opt) {
        return CF_NULL_PTR;
    }

    opts = self->opts;
    while (opts) {
        if (cf_strlen(opt) == 1 && opt[0] == opts->short_name) {
            return opts;
        } else if (cf_string_len(&opts->long_name) > 0
            && cf_strcmp(cf_string_ptr(&opts->long_name), opt) == 0) {
            return opts;
        }
        opts = opts->next;
    }

    return CF_NULL_PTR;
}

cf_bool_t cfx_cli_cmd_add_sub(cfx_cli_cmd_t* self, cfx_cli_cmd_t* subcmd) {
    cfx_cli_cmd_t* cmd = CF_NULL_PTR;
    if (!subcmd) {
        return CF_FALSE;
    }
    if (!self->child) {
        self->child = subcmd;
        self->child->prev = CF_NULL_PTR;
        self->child->next = CF_NULL_PTR;
    }
    else {
        cmd = self->child;
        while (cmd->next) {
            cmd = cmd->next;
        }
        subcmd->prev = cmd;
        subcmd->next = CF_NULL_PTR;
        cmd->next = subcmd;
    }
    return CF_TRUE;
}

cf_bool_t cfx_cli_cmd_add_opt(
    cfx_cli_cmd_t* self,
    char short_name,
    const char* long_name,
    const char* desc,
    cf_bool_t required,
    cf_bool_t flag,
    const char* defvalue) {
    cfx_cli_opt_t* opt = CF_NULL_PTR;
    cfx_cli_opt_t* it;
    if (short_name == 0 
        && (long_name == CF_NULL_PTR || cf_strlen(long_name) == 0)) {
        return CF_FALSE;
    }
    if (!desc) {
        return CF_FALSE;
    }

    opt = cf_malloc_z(sizeof(cfx_cli_opt_t));

    opt->short_name = short_name;
    if (long_name) {
        cf_string_init(&opt->long_name, cf_strlen(long_name), long_name);
    }
    opt->next = CF_NULL_PTR;
    opt->required = required;
    opt->flag = flag;
    if (defvalue) {
        cf_string_init(&opt->value, cf_strlen(defvalue), defvalue);
    }
    
    if (!self->opts) {
        self->opts = opt;
    }
    else {
        it = self->opts;
        while (it->next) {
            it = it->next;
        }
        it->next = opt;
    }
    
    return CF_TRUE;
}
