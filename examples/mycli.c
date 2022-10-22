#include "cfx/cli.h"
#include <stdio.h>

int cmd_proc(const cfx_cli_t* cli, cf_void_t* sess, cfx_cli_opt_t* opts, cf_size_t argc, cf_char_t* argv[]) {
    printf("cmd1 proc cnt: %d argv0: %s \n", argc, argv[0]);
    return 0;
}


int main(int argc, char* argv[]) {
    cfx_cli_t cli;
    cfx_cli_cmd_t subcmd1, subcmd11, *cmd;
    cfx_cli_opt_t opt_a, opt_b, opt_c, opt_d;

    cfx_cli_init(&cli, CF_NULL_PTR, "mycli", "this is a simple CLI", "1.0.0");
    cmd = cfx_cli_root_cmd(&cli);

    cfx_cli_cmd_init(&subcmd1, "cmd1", "help for cmd1", CF_NULL_PTR);
    cfx_cli_cmd_init(&subcmd11, "cmd11", "help for cmd11", cmd_proc);

    cfx_cli_opt_init(&opt_a, 'a', "aaa", "help for option a", CF_TRUE, CF_FALSE, "");
    cfx_cli_opt_init(&opt_b, 'b', "bbb", "help for option b", CF_TRUE, CF_FALSE, "");
    cfx_cli_opt_init(&opt_c, 'c', "", "help for option c", CF_TRUE, CF_TRUE, "");
    cfx_cli_opt_init(&opt_d, 'd', "ddd", "help for option d", CF_TRUE, CF_TRUE, "");

    cfx_cli_cmd_add_opt(&subcmd11, &opt_a);
    cfx_cli_cmd_add_opt(&subcmd11, &opt_b);
    cfx_cli_cmd_add_opt(&subcmd11, &opt_c);
    cfx_cli_cmd_add_opt(&subcmd11, &opt_d);

    cfx_cli_cmd_add_sub(&subcmd1, &subcmd11);
    cfx_cli_cmd_add_sub(cmd, &subcmd1);

    cfx_cli_input(&cli, CF_NULL_PTR, argc, argv);
    cfx_cli_deinit(&cli);
    return 0;
}