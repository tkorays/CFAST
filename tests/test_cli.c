#include "cf/types.h"
#include "cfx/cli.h"
#include "cf/str.h"
#include "cf/assert.h"
#include <stdio.h>

int cmd_proc(const cfx_cli_t* cli, cf_void_t* sess, cfx_cli_cmd_t* cmd, cf_size_t argc, cf_char_t* argv[]) {
    printf("cmd1 proc cnt: %zu argv0: %s \n", argc, argv[0]);
    cf_assert(cfx_cli_opt_exist(cmd, "a") == CF_TRUE);
    cf_assert(cfx_cli_opt_exist(cmd, "aaa") == CF_TRUE);
    cf_assert(cfx_cli_opt_exist(cmd, "s") == CF_FALSE);
    cf_assert(cfx_cli_opt_exist(cmd, "sss") == CF_FALSE);
    cf_assert(cf_strcmp(cfx_cli_opt_val(cmd, "a"), "abc") == 0);
    cf_assert(cfx_cli_opt_val(cmd, "ddd") == CF_NULL_PTR);
    printf("a: %s, b: %s\n", cfx_cli_opt_val(cmd, "a"), cfx_cli_opt_val(cmd, "bbb"));
    return 0;
}

int main(int argc, char const *argv[]) {
    cfx_cli_t* cli;
    cfx_cli_cmd_t *cmd = CF_NULL_PTR, *cmd1 = CF_NULL_PTR, * cmd11 = CF_NULL_PTR;

    cli = cfx_cli_new("mycli", "this is a simple CLI", "1.0.0");

    cmd = cfx_cli_get_root_cmd(cli);
    cmd1 = cfx_cli_cmd_new("cmd1", "help for cmd1", CF_NULL_PTR);
    cmd11 = cfx_cli_cmd_new("cmd11", "help for cmd1", cmd_proc);

    cfx_cli_cmd_add_sub(cmd, cmd1);
    cfx_cli_cmd_add_sub(cmd1, cmd11);

    cfx_cli_opt_add(cmd11, 'a', "aaa", "help for option a", CFX_CLI_OPT_REQ, "");
    cfx_cli_opt_add(cmd11, 'b', "bbb", "help for option b", CFX_CLI_OPT_REQ, "");
    cfx_cli_opt_add(cmd11, 'c', "", "help for option c", CFX_CLI_OPT_REQ | CFX_CLI_OPT_IS_FLAG, "");
    cfx_cli_opt_add(cmd11, 'd', "ddd", "help for option d", CFX_CLI_OPT_REQ | CFX_CLI_OPT_IS_FLAG, "");

    int argc1 = 11;
    char* argv1[] = {
        "mycli", "cmd1", "cmd11", "-a", "abc", "--bbb", "bbb", "-c", "--ddd",
        "1234", "5678"
    };

    cfx_cli_input(cli, CF_NULL_PTR, argc1, argv1);
    cfx_cli_delete(cli);
    return 0;
}
