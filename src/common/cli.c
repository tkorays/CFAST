#include <cf/cli.h>
#include <cf/memory.h>
#include <cf/str.h>
#include <cf/mpool.h>
#include <cf/err.h>
#include <stdio.h>
#include <string.h>

#define CHECK_CLI_STATUS(cli, p) do {if(!cli || !p) return CF_EPARAM;\
    if(cli->pos >= cli->vcmd_size) return CF_NOK;\
    if(cli->cmd[cli->pos] == CF_NULL_PTR) return CF_NOK;} while(0)
/*
cf_errno_t  cf_cli_get_char(cf_cli_t* cli, cf_char_t* p) {
    CHECK_CLI_STATUS(cli, p);
    *p = cli->cmd[0];
    return CF_OK;
}
cf_errno_t  cf_cli_get_uchar(cf_cli_t* cli, cf_uchar_t* p) {
    CHECK_CLI_STATUS(cli, p);
    *p = (cf_uchar_t)cli->cmd[0];
    return CF_OK;
}
cf_errno_t  cf_cli_get_short(cf_cli_t* cli, cf_short_t* p) {
    CHECK_CLI_STATUS(cli, p);
    
    return CF_OK;
}
cf_errno_t  cf_cli_get_ushort(cf_cli_t* cli, cf_ushort_t* p) {
    if(!cli || !p) return CF_EPARAM;
    if(cli->pos >= cli->vcmd_size) return CF_NOK;
}
cf_errno_t  cf_cli_get_int(cf_cli_t* cli, cf_int_t* p) {
    if(!cli || !p) return CF_EPARAM;
    if(cli->pos >= cli->vcmd_size) return CF_NOK;
}
cf_errno_t  cf_cli_get_uint(cf_cli_t* cli, cf_uint_t* p) {
    if(!cli || !p) return CF_EPARAM;
    if(cli->pos >= cli->vcmd_size) return CF_NOK;
}
cf_errno_t  cf_cli_get_int64(cf_cli_t* cli, cf_int64_t* p) {
    if(!cli || !p) return CF_EPARAM;
    if(cli->pos >= cli->vcmd_size) return CF_NOK;
}
cf_errno_t  cf_cli_get_uint16(cf_cli_t* cli, cf_uint64_t* p) {
    if(!cli || !p) return CF_EPARAM;
    if(cli->pos >= cli->vcmd_size) return CF_NOK;
}
cf_errno_t  cf_cli_get_str(cf_cli_t* cli, cf_char_t* p, cf_size_t size) {
    if(!cli || !p) return CF_EPARAM;
    if(cli->pos >= cli->vcmd_size) return CF_NOK;
}
*/


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
    cmd = cmd->child;
    while(cmd) {
        prev = cmd;
        cmd = cmd->next;
    }
    // no child
    if(!prev) {
        cmd->child = cf_mpool_alloc(cli->pool, sizeof(cf_cli_cmd_t));
        prev->next->next = CF_NULL_PTR;
        prev->next->prev = CF_NULL_PTR;
        prev->next->child = CF_NULL_PTR;
        prev->next->name = name;
        prev->next->func = CF_NULL_PTR;
    }
    else {
        // 在最后一个节点后添加
        prev->next = cf_mpool_alloc(cli->pool, sizeof(cf_cli_cmd_t));
        prev->next->next = CF_NULL_PTR;
        prev->next->prev = prev;
        prev->next->child = CF_NULL_PTR;
        prev->next->name = name;
        prev->next->func = CF_NULL_PTR;
    }
    return prev->next;
}


static cf_errno_t cf_cli_show_help(cf_char_t* s, cf_void_t* arg) {
    cf_cli_t* cli = (cf_cli_t*)arg;
    if(!cli->output) return CF_OK;
    cli->output("help:\n");
    return CF_OK;
}

CF_DECLARE(cf_errno_t) cf_cli_init(cf_cli_t* cli, cf_cli_cfg_t* cfg) {
    cf_cli_cmd_t* cmd = CF_NULL_PTR;
    if(!cli || !cfg) return CF_EPARAM;
    cli->output = cfg->output;
    cli->pool = cf_mpool_create(1024);
    if(cli->pool == CF_NULL_PTR) return CF_NOK;

    /* cli默认带一个?命令，作为链表头部 */
    cmd = (cf_cli_cmd_t*)cf_mpool_alloc(cli->pool, sizeof(cf_cli_cmd_t));
    if(!cmd)  {
        cf_mpool_destroy(cli->pool);
        return CF_NOK;
    }

    cmd->prev = CF_NULL_PTR;
    cmd->next = CF_NULL_PTR;
    cmd->child = CF_NULL_PTR;
    cmd->name = "?";
    cmd->func = cf_cli_show_help;
    
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

CF_DECLARE(cf_errno_t) cf_cli_input(cf_cli_t* cli, cf_char_t* line) {
    if(!cli || !line) return CF_EPARAM;
    return CF_OK;
}
CF_DECLARE(cf_errno_t) cf_cli_register(cf_cli_t* cli, cf_char_t* cmd, cf_errno_t(*func)(cf_char_t*, cf_void_t*)) {
    cf_char_t* buf;
    cf_char_t* name;
    cf_size_t size;
    cf_size_t i = 0;
    cf_cli_cmd_t* c = CF_NULL_PTR;

    cf_cli_cmd_t* root = cli->cmds;
    if(!cli || !cmd || !func) return CF_EPARAM;

    /* 申请内存用于存储字符串 */
    size = cf_strlen(cmd)+2;
    buf = cf_mpool_alloc(cli->pool, size);
    (cf_void_t)cf_memset_s(buf, size, 0, size);
    cf_strcpy_s(buf, size, cmd);

    name = buf;
    while(buf[i] != '\0') {
        if(CF_IS_SPACE(buf[i])) {
            buf[i] = '\0';
            c = find_cmd_in_child(root, name);
            if(c == CF_NULL_PTR) {
                // 子节点中没有找到，则新建一个cmd
                c = add_child_node(cli, root, name);
            }
            root = c;
            name = buf+i+1;
        }
        i++;
    }

    if(root) {
        root->func = func;
    }
    return CF_OK;
}