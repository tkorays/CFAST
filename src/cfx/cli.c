#include "cfx/cli.h"
#include "cf/str.h"
#include <stdio.h>
#include <stdarg.h>
#include "cf/optional.h"


#define CFX_DEF_CLI_LICENSE "CFAST CLI v1.0, Author(tkorays), All right reserved!"

void _cfx_cli_io_output_fn(cf_void_t* ctx, const cf_char_t* fmt, ...) {
    va_list args;
    cf_char_t buf[1024] = {0};
    va_start(args, fmt);
    vsnprintf(buf, sizeof(buf), fmt, args);
    va_end(args);
    printf("%s", buf);
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

void cfx_cli_help(cfx_cli_t* self, cfx_cli_cmd_t* cmd) {
    cfx_cli_cmd_t* p = &self->root;
    cfx_cli_opt_t* opt;
    char buf[2048];
    int i = 0;
    i += cf_snprintf(buf + i, sizeof(buf) - i, "Usage: ");
    while (p && p != cmd) {
        i += cf_snprintf(buf + i, sizeof(buf) - i, "%s ", cf_string_ptr(&p->name));
        p = p->next;
    }
    i += cf_snprintf(buf + i, sizeof(buf) - i, "%s ", cf_string_ptr(&cmd->name));
    
    i += cf_snprintf(buf + i, sizeof(buf) - i, "COMMAND [OPTIONS] [ARGS] ...\n");
    i += cf_snprintf(buf + i, sizeof(buf) - i, "\n    %s\n", cf_string_ptr(&cmd->desc));
    self->io.output(CF_NULL_PTR, "%s", buf);

    p = cmd->child;
    if (p) {
        self->io.output(CF_NULL_PTR, "\nCommands:\n");
        while (p) {
            self->io.output(CF_NULL_PTR, "  %-18s%s\n",
                cf_string_ptr(&p->name), cf_string_ptr(&p->desc));
            p = p->next;
        }
    }

    self->io.output(CF_NULL_PTR, "\nOptions:\n");
    if (cmd == &self->root) {
        self->io.output(CF_NULL_PTR, "  --version           Show the version and exit.\n");
    }
    self->io.output(CF_NULL_PTR, "  --help              Show this message and exit.\n");

    opt = cmd->opts;
    while (opt) {
        if (opt->short_name && cf_string_len(&opt->long_name)) {
            self->io.output(CF_NULL_PTR, "  -%c,--%-15s%s%s%s\n",
                opt->short_name,
                cf_string_ptr(&opt->long_name),
                cf_string_ptr(&opt->desc),
                opt->flag ? " [flag]" : "",
                opt->required ? " [required]" : "");
        }
        else if (opt->short_name) {
            self->io.output(CF_NULL_PTR, "  -%-19c%s%s%s\n",
                opt->short_name,
                cf_string_ptr(&opt->desc),
                opt->flag ? " [flag]" : "",
                opt->required ? " [required]" : "");
        }
        else {
            self->io.output(CF_NULL_PTR, "  --%-18s%s%s%s\n",
                cf_string_ptr(&opt->long_name),
                cf_string_ptr(&opt->desc),
                opt->flag ? " [flag]" : "",
                opt->required ? " [required]" : "");
        }
        opt = opt->next;
    }
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
                    self->io.output(CF_NULL_PTR, "no such comamnd '%s', try '--help' for help\n", argv[i]);
                    return CF_FALSE;
                    ///* leave command state and enter argument state */
                    //cmd = parent;
                    //state = CLI_STATE_ARG;
                    //substate = CLI_OPT_SUBSTATE_WAITING_FOR_OPT;
                    //break;
                } else {
                    parent = cmd;
                }
            } else {
                /* from command state to option state */
                state = CLI_STATE_OPT;
                substate = CLI_OPT_SUBSTATE_WAITING_FOR_OPT;
                --i;
                continue;
            }
        } else if (state == CLI_STATE_OPT) {
            if (!cmd) {
                self->io.output(CF_NULL_PTR, "command not found!\n");
                return CF_FALSE;
            }

            if (is_opt) {
                    /* we are waiting for a value and prev option is not a flag, error state */
                if (substate == CLI_OPT_SUBSTATE_WAITING_FOR_VAL && opt && !opt->flag) {
                    if (cf_string_len(&opt->long_name)) {
                      self->io.output(
                          CF_NULL_PTR, "expect a option value for %s\n", opt->long_name.ptr);
                    } else {
                      self->io.output(
                          CF_NULL_PTR, "expect a option value for %c\n", opt->short_name);
                    }
                    return CF_FALSE;
                }

                /* this is an option we should wait for value if option requires value */
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
                    if (cf_strcmp(argv[i], "--help") == 0) {
                        cfx_cli_help(self, cmd);
                    } else if (cmd == &self->root && cf_strcmp(argv[i], "--version") == 0) {
                        self->io.output(CF_NULL_PTR, "%s\n%s version %s\n",
                            CFX_DEF_CLI_LICENSE, cf_string_ptr(&self->name), cf_string_ptr(&self->version));
                    } else {
                        self->io.output(CF_NULL_PTR, "no option named %s", argv[i]);
                    }
                    return CF_FALSE;
                }
                /* flag option don't need a value */
                opt->has_value = opt->flag ? CF_TRUE : CF_FALSE;
                substate = opt->flag ? CLI_OPT_SUBSTATE_WAITING_FOR_OPT : CLI_OPT_SUBSTATE_WAITING_FOR_VAL;
            } else if (substate == CLI_OPT_SUBSTATE_WAITING_FOR_OPT && !is_opt) {
                /* no pending options, enter argument state */
                state = CLI_STATE_ARG;
                substate = CLI_OPT_SUBSTATE_WAITING_FOR_OPT;
                break;
            } else if (substate == CLI_OPT_SUBSTATE_WAITING_FOR_VAL && !is_opt) {
                opt->has_value = CF_TRUE;
                cf_string_init_with_ref(&opt->value, argv[i], cf_strlen(argv[i]));
                substate = CLI_OPT_SUBSTATE_WAITING_FOR_OPT;
            }
        }
    }

    if (!cmd) {
        self->io.output(CF_NULL_PTR, "command not found!");
        return CF_FALSE;
    }
    if (!cmd->proc) {
        cfx_cli_help(self, cmd);
        return CF_FALSE;
    }

    /* check if all required options is set */
    opt = cmd->opts;
    while (opt) {
        if (opt->required && !opt->has_value) {
            if (opt->short_name) {
                self->io.output(CF_NULL_PTR, "missing option -%c! use --help to get options", opt->short_name);
            } else {
                self->io.output(CF_NULL_PTR, "missing option -%s! use --help to get options", cf_string_ptr(&opt->long_name));
            }
            return CF_FALSE;
        }
        opt = opt->next;
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
    if (!name || !desc) {
        return CF_FALSE;
    }

    cf_membzero(self, sizeof(cfx_cli_cmd_t));
    cf_string_init(&self->name, cf_strlen(name), name);
    cf_string_init(&self->desc, cf_strlen(desc), desc);
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
        cfx_cli_opt_deinit(opt);
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
        child = child->next;
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

cf_bool_t cfx_cli_cmd_add_opt(cfx_cli_cmd_t* self, cfx_cli_opt_t* opt) {
    cfx_cli_opt_t* it;
    if (!opt) return CF_FALSE;

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

cf_bool_t cfx_cli_opt_init(cfx_cli_opt_t* self, char sn, const char* ln, const char* desc, cf_bool_t required, cf_bool_t flag, const char* def) {
    if (sn == 0 && (ln == CF_NULL_PTR || cf_strlen(ln) == 0)) {
        return CF_FALSE;
    }
    if (!desc) {
        return CF_FALSE;
    }
    cf_membzero(self, sizeof(cfx_cli_opt_t));

    self->short_name = sn;
    if (ln) {
        cf_string_init(&self->long_name, cf_strlen(ln), ln);
    }
    cf_string_init(&self->desc, cf_strlen(desc), desc);
    self->next = CF_NULL_PTR;
    self->required = required;
    self->flag = flag;
    if (def) {
        cf_string_init(&self->value, cf_strlen(def), def);
    }
    return CF_FALSE;
}

void cfx_cli_opt_deinit(cfx_cli_opt_t* self) {
    cf_string_deinit(&self->long_name);
    cf_string_deinit(&self->desc);
    cf_string_deinit(&self->value);
}

cf_bool_t cfx_cli_opt_exist_sn(cfx_cli_opt_t* opt, char short_name) {
    if (short_name == 0) return CF_FALSE;

    while (opt) {
        if (opt->short_name == short_name) return CF_TRUE;
        opt = opt->next;
    }
    return CF_FALSE;
}

cf_bool_t cfx_cli_opt_exist_ln(cfx_cli_opt_t* opt, const char* long_name) {
    if (long_name == CF_NULL_PTR) return CF_FALSE;

    while (opt) {
        if (cf_string_len(&opt->long_name) == 0) {
            opt = opt->next;
            continue;
        }
        if (cf_strcmp(cf_string_ptr(&opt->long_name), long_name) == 0) return CF_TRUE;
        opt = opt->next;
    }
    return CF_FALSE;
}

cf_bool_t cfx_cli_opt_has_val_sn(cfx_cli_opt_t* opt, char short_name) {
    if (short_name == 0) return CF_FALSE;

    while (opt) {
        if (opt->short_name == short_name && opt->has_value) return CF_TRUE;
        opt = opt->next;
    }
    return CF_FALSE;
}

cf_bool_t cfx_cli_opt_has_val_ln(cfx_cli_opt_t* opt, const char* long_name) {
    if (long_name == CF_NULL_PTR) return CF_FALSE;

    while (opt) {
        if (cf_string_len(&opt->long_name) == 0) continue;
        if (cf_strcmp(cf_string_ptr(&opt->long_name), long_name) == 0  && opt->has_value) return CF_TRUE;
        opt = opt->next;
    }
    return CF_FALSE;
}
