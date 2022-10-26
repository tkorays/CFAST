#include "cf/types.h"
#include "cfx/cli.h"
#include <stdio.h>

int cmd_proc(const cfx_cli_t* cli, cf_void_t* sess, cfx_cli_cmd_t* cmd, cf_size_t argc, cf_char_t* argv[]) {
    printf("cmd1 proc cnt: %zu argv0: %s \n", argc, argv[0]);
    return 0;
}

int main(int argc, char* argv[]) {
    cfx_cli_t* cli;
    cfx_cli_cmd_t *cmd = CF_NULL_PTR, *cmd1 = CF_NULL_PTR, * cmd11 = CF_NULL_PTR;

    cli = cfx_cli_new(CF_NULL_PTR, "mycli", "this is a simple CLI", "1.0.0");

    cmd = cfx_cli_root_cmd(cli);
    cmd1 = cfx_cli_cmd_add(cmd, "cmd1", "help for cmd1", CF_NULL_PTR);
    cmd11 = cfx_cli_cmd_add(cmd1, "cmd11", "help for cmd1", cmd_proc);

    cfx_cli_opt_add(cmd11, 'a', "aaa", "help for option a", CFX_CLI_OPT_REQ, "");
    cfx_cli_opt_add(cmd11, 'b', "bbb", "help for option b", CFX_CLI_OPT_REQ, "");
    cfx_cli_opt_add(cmd11, 'c', "", "help for option c", CFX_CLI_OPT_REQ | CFX_CLI_OPT_IS_FLAG, "");
    cfx_cli_opt_add(cmd11, 'd', "ddd", "help for option d", CFX_CLI_OPT_REQ | CFX_CLI_OPT_IS_FLAG, "");

    cfx_cli_input(cli, CF_NULL_PTR, argc, argv);
    cfx_cli_delete(cli);
    return 0;
}
