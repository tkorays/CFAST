#include "cfx/cli.h"
#include "cf/memory.h"
#include "cf/str.h"
#include <stdio.h>
#include <stdarg.h>
#include "cf/optional.h"
#include "cf/types.h"
#include "cfx/ascii_art.h"


#define CFX_DEF_CLI_LICENSE "CFAST CLI v1.0, Author(tkorays), All right reserved!"

/** see `cfx_cli_io_t` */
struct cfx_cli_io {
    cfx_cli_io_output_fn  output;   /** output function for CLI */
};

/** see `cfx_cli_opt_t` */
struct cfx_cli_opt {
    char                short_name; /** short name of this option */
    cf_string_t         long_name;  /** long name of this option */
    cf_string_t         desc;       /** description of this option */
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

void cfx_cli_cmd_deinit(cfx_cli_cmd_t* self);
void cfx_cli_opt_deinit(cfx_cli_opt_t* self);

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

cfx_cli_t* cfx_cli_new(cfx_cli_io_t* io, const char* name, const char* desc, const char* ver) {
    cfx_cli_t* self;
    if (!name || !desc || !ver || cf_strlen(name) == 0 || cf_strlen(desc) == 0 || cf_strlen(ver) == 0) {
        return CF_NULL_PTR;
    }

    self = (cfx_cli_t*)cf_malloc_z(sizeof(cfx_cli_t));
    if (!self) {
        return CF_NULL_PTR;
    }

    self->name      = cf_string_dup_c(name, cf_strlen(name));
    self->desc      = cf_string_dup_c(desc, cf_strlen(desc));
    self->version   = cf_string_dup_c(ver, cf_strlen(ver));
    self->root.name = cf_string_ref(&self->name);
    self->root.desc = cf_string_ref(&self->desc);

    if (!io) {
        self->io    = _cfx_cli_default_io_;
    } else {
        self->io    = *io;
    }
    return self;
}

void cfx_cli_delete(cfx_cli_t* self) {
    cf_string_deinit(&self->name);
    cf_string_deinit(&self->desc);
    cf_string_deinit(&self->version);
    cfx_cli_cmd_deinit(&self->root);
    self->root.prev = CF_NULL_PTR;
    self->root.next = CF_NULL_PTR;
    self->root.child = CF_NULL_PTR;
    self->root.opts = CF_NULL_PTR;
    cf_free(self);
}

void cfx_cli_help(cfx_cli_t* self, cfx_cli_cmd_t* cmd) {
    cfx_cli_cmd_t* p = &self->root;
    cfx_cli_opt_t* opt;
    char buf[2048];
    int i = 0;
    i += cfx_art_print(cf_string_ptr(&p->name), buf, sizeof(buf) - i, CFX_ART_FONT_SLANT);
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
                opt->value = cf_string_ref_c(argv[i], cf_strlen(argv[i]));
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
        cmd->proc(self, CF_NULL_PTR, cmd, argc - i, &argv[i]);
    }
    return CF_TRUE;
}

cfx_cli_cmd_t* cfx_cli_root_cmd(cfx_cli_t* self) {
    return &self->root;
}

cfx_cli_cmd_t* cfx_cli_cmd_add(cfx_cli_cmd_t* parent, const char* name, const char* desc, cfx_cli_proc_fn fn) {
    cfx_cli_cmd_t* cmd = CF_NULL_PTR, *p;
    if (!name || !desc) {
        return CF_FALSE;
    }

    cmd = cf_malloc_z(sizeof(cfx_cli_cmd_t));
    if (!cmd) return CF_NULL_PTR;


    cmd->name   = cf_string_dup_c(name, cf_strlen(name));
    cmd->desc   = cf_string_dup_c(desc, cf_strlen(desc));
    cmd->proc = fn;

    if (!parent->child) {
        parent->child = cmd;
        parent->child->prev = CF_NULL_PTR;
        parent->child->next = CF_NULL_PTR;
    }
    else {
        p = parent->child;
        while (p->next) {
             p= p->next;
        }
        cmd->prev = p;
        cmd->next = CF_NULL_PTR;
        p->next = cmd;
    }
    return cmd;
}

cf_bool_t cfx_cli_cmd_init(cfx_cli_cmd_t* self, const char* name, const char* desc, cfx_cli_proc_fn fn) {
    if (!name || !desc) {
        return CF_FALSE;
    }

    cf_membzero(self, sizeof(cfx_cli_cmd_t));
    self->name  = cf_string_dup_c(name, cf_strlen(name));
    self->desc  = cf_string_dup_c(desc, cf_strlen(desc));
    self->proc = fn;

    return CF_TRUE;
}

void cfx_cli_cmd_deinit(cfx_cli_cmd_t* self) {
    cfx_cli_cmd_t* cmd, *cmd_next;
    cfx_cli_opt_t* opt, *opt_next;
    if (!self) return;

    /* deinit child */
    cfx_cli_cmd_deinit(self->child);
    cf_free(self->child);
    self->child = CF_NULL_PTR;

    
    /* deinit sublings */
    cmd = self->next;
    while (cmd) {
        cmd_next = cmd->next;
        cfx_cli_cmd_deinit(cmd);
        cf_free(cmd);
        cmd = cmd_next;
    }

    /* deinit self */
    cf_string_deinit(&self->name);
    cf_string_deinit(&self->desc);

    opt = self->opts;
    while (opt) {
        opt_next = opt->next;
        cfx_cli_opt_deinit(opt);
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


cfx_cli_opt_t* cfx_cli_opt_add(cfx_cli_cmd_t* cmd, char sn, const char* ln, const char* desc, cf_uint32_t flags, const char* def) {
    cfx_cli_opt_t* opt = CF_NULL_PTR, *it;
    if (sn == 0 && (ln == CF_NULL_PTR || cf_strlen(ln) == 0)) {
        return CF_NULL_PTR;
    }
    if (!desc) {
        return CF_NULL_PTR;
    }
    opt = cf_malloc_z(sizeof(cfx_cli_opt_t));
    opt->short_name = sn;
    if (ln) {
        opt->long_name  = cf_string_dup_c(ln, cf_strlen(ln));
    }
    opt->desc   = cf_string_dup_c(desc, cf_strlen(desc));
    opt->next = CF_NULL_PTR;
    opt->required = (flags & CFX_CLI_OPT_REQ) ? CF_TRUE : CF_FALSE;
    opt->flag = (flags & CFX_CLI_OPT_IS_FLAG) ? CF_TRUE : CF_FALSE;
    if (def) {
        opt->value  = cf_string_dup_c(def, cf_strlen(def));
    }

    if (!cmd->opts) {
        cmd->opts = opt;
    }
    else {
        it = cmd->opts;
        while (it->next) {
            it = it->next;
        }
        it->next = opt;
    }
    return opt;
}

void cfx_cli_opt_deinit(cfx_cli_opt_t* self) {
    cf_string_deinit(&self->long_name);
    cf_string_deinit(&self->desc);
    cf_string_deinit(&self->value);
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


cf_char_t* cfx_cli_opt_val(cfx_cli_cmd_t* self, char* name) {
    cfx_cli_opt_t* opt = cfx_cli_cmd_opt(self, name);
    if (opt && opt->has_value) {
        return opt->value.ptr;
    }
    return CF_NULL_PTR;
}
