#include <gtest/gtest.h>
#include <cfx/cli.h>
#include <stdio.h>

int cmd_proc(const cfx_cli_t* cli, cf_void_t* sess, cfx_cli_opt_t* opts, cf_size_t argc, cf_char_t* argv[]) {
    printf("cmd1 proc cnt: %d argv0: %s \n", argc, argv[0]);
    return 0;
}

TEST(cfx_cli_test, init) {
    cfx_cli_t cli;
    cfx_cli_init(&cli, CF_NULL_PTR, "mycli", "this is a simple CLI", "1.0.0");

    // get the root command
    cfx_cli_cmd_t* cmd = cfx_cli_root_cmd(&cli);

    // create a new subcommand and add to root
    cfx_cli_cmd_t subcmd1;
    cfx_cli_cmd_init(&subcmd1, "cmd1", "cmd1 help", cmd_proc);
    cfx_cli_cmd_t subcmd11;
    cfx_cli_cmd_init(&subcmd11, "cmd11", "cmd11 help", cmd_proc);
    cfx_cli_cmd_add_opt(&subcmd11, 'a', "aaa", "option a", CF_TRUE, CF_FALSE, "");
    cfx_cli_cmd_add_opt(&subcmd11, 'b', "bbb", "option b", CF_TRUE, CF_FALSE, "");
    cfx_cli_cmd_add_opt(&subcmd11, 'c', "", "option c", CF_TRUE, CF_TRUE, "");
    cfx_cli_cmd_add_opt(&subcmd11, 'd', "ddd", "option d", CF_TRUE, CF_TRUE, "");
    cfx_cli_cmd_add_sub(&subcmd1, &subcmd11);
    cfx_cli_cmd_add_sub(cmd, &subcmd1);

    int argc = 11;
    char* argv[] = {
        "mycli", "cmd1", "cmd11", "-a", "abc", "--bbb", "bbb", "-c", "--ddd",
        "1234", "5678"
    };

    cfx_cli_input(&cli, CF_NULL_PTR, argc, argv);

    cfx_cli_deinit(&cli);
}
