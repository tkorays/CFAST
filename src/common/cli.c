#include <cf/cli.h>
#include <cf/memory.h>
#include <cf/str.h>
#include <cf/mpool.h>
#include <cf/err.h>
#include <stdio.h>
#include <string.h>

static cf_cli_cmd_t* find_cmd_in_child(cf_cli_cmd_t* cmd, cf_char_t* name) {
    cf_cli_cmd_t* c = cmd->child;
    // 内部函数，不检查参数
    while(c) {
        if(cf_strcmp(name, c->name) == 0) return c;
        c = c->next;
    }
    return CF_NULL_PTR;
}

static cf_cli_cmd_t* add_child_node(cf_cli_t* cli, cf_cli_cmd_t* cmd, cf_char_t* name) {
    // 内部函数，不检查参数
    cf_cli_cmd_t* prev = CF_NULL_PTR;
    cf_cli_cmd_t* cur = cmd->child;
    while(cur) {
        prev = cur;
        cur = cur->next;
    }
    // no child
    if(!prev) {
        cmd->child = cf_mpool_alloc(cli->pool, sizeof(cf_cli_cmd_t));
        cmd->child->next = CF_NULL_PTR;
        cmd->child->prev = CF_NULL_PTR;
        cmd->child->child = CF_NULL_PTR;
        cmd->child->name = name;
        cmd->child->func = CF_NULL_PTR;
        return cmd->child;
    }
    else {
        // 在最后一个节点后添加
        prev->next = cf_mpool_alloc(cli->pool, sizeof(cf_cli_cmd_t));
        prev->next->next = CF_NULL_PTR;
        prev->next->prev = prev;
        prev->next->child = CF_NULL_PTR;
        prev->next->name = name;
        prev->next->func = CF_NULL_PTR;
        return prev->next;
    }
}

static cf_char_t* get_cmd_name(cf_mpool_t* pool, cf_char_t* s, cf_size_t* pos) {
    cf_size_t start = *pos;
    cf_char_t* name = CF_NULL_PTR;

    // 跳过空格
    while(CF_IS_SPACE(s[start])) {
        ++start;
        ++(*pos);
    }

    while(!CF_IS_SPACE(s[*pos]) && s[*pos] != '\0') ++(*pos);

    if(*pos <= start) return CF_NULL_PTR;

    name = cf_mpool_alloc(pool, (*pos - start + 1));
    cf_memset_s(name, (*pos - start + 1), 0, (*pos - start + 1));
    cf_memcpy_s(name, (*pos - start + 1), s + start, (*pos - start));
    return name;
}

static cf_errno_t cf_cli_god_cmd(cf_size_t argc, cf_char_t* argv[]) {
    // do nothing
    return CF_OK;
}

CF_DECLARE(cf_errno_t) cf_cli_init(cf_cli_t* cli, cf_cli_cfg_t* cfg) {
    cf_cli_cmd_t* cmd = CF_NULL_PTR;
    if(!cli || !cfg) return CF_EPARAM;
    cli->output = cfg->output;
    cli->pool = cf_mpool_create(1024);
    if(cli->pool == CF_NULL_PTR) return CF_NOK;

    /* cli默认带一个?命令，吾乃命令之祖 */
    cmd = (cf_cli_cmd_t*)cf_mpool_alloc(cli->pool, sizeof(cf_cli_cmd_t));
    if(!cmd)  {
        cf_mpool_destroy(cli->pool);
        return CF_NOK;
    }

    cmd->prev = CF_NULL_PTR;
    cmd->next = CF_NULL_PTR;
    cmd->child = CF_NULL_PTR;
    cmd->name = "";
    cmd->func = cf_cli_god_cmd;
    
    cli->cmds = cmd;
    return CF_OK;
}

CF_DECLARE(cf_errno_t) cf_cli_uninit(cf_cli_t* cli) {
    if(!cli) return CF_EPARAM;
    if(cli->pool) cf_mpool_destroy(cli->pool);
    return CF_OK;
}

static cf_void_t output_str(cf_char_t* s) {
    printf("%s", s);
}
CF_DECLARE(cf_void_t)  cf_cli_cfg_default(cf_cli_cfg_t* cfg) {
    if(!cfg) return ;
    cfg->output = output_str;
}

CF_DECLARE(cf_errno_t) cf_cli_input(cf_cli_t* cli, cf_size_t argc, cf_char_t* argv[]) {
    cf_cli_cmd_t* root = cli->cmds;
    cf_cli_cmd_t* c = CF_NULL_PTR;
    cf_cli_cmd_t* pc = CF_NULL_PTR; // parent command
    cf_size_t i;
    if(!cli || !argc || !argv) return CF_EPARAM;

    for(i = 0; i < argc; i++) {
        c = find_cmd_in_child(root, argv[i]);
        if(c == CF_NULL_PTR) break;
        root = c;
    }

    if(!root->func) {
        cli->output("Command Not Fit!");
        return CF_NOK;
    } else {
        return root->func(argc - i, argv + i);
    }
}

CF_DECLARE(cf_errno_t) cf_cli_register(cf_cli_t* cli, cf_char_t* cmd, cf_errno_t(*func)(cf_size_t argc, cf_char_t* argv[])) {
    cf_char_t* buf;
    cf_char_t* name;
    cf_cli_cmd_t* c = CF_NULL_PTR;
    cf_size_t pos = 0;

    cf_cli_cmd_t* root = cli->cmds;
    if(!cli || !cmd || !func) return CF_EPARAM;

    while((name = get_cmd_name(cli->pool, cmd, &pos))) {
        c = find_cmd_in_child(root, name);
        if(c == CF_NULL_PTR) {
            // 子节点中没有找到，则新建一个cmd
            c = add_child_node(cli, root, name);
        }
        root = c;
    }

    if(root) {
        root->func = func;
    }
    return CF_OK;
}
