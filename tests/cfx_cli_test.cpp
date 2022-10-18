#include <gtest/gtest.h>
#include <cfx/cli.h>
#include <stdio.h>

int cmd_proc(const cfx_cli_t* cli, cf_void_t* sess, cfx_cli_opt_t* opts, cf_size_t argc, cf_char_t* argv[]) {
    printf("cmd1 proc cnt: %d argv0: %s \n", argc, argv[0]);
    cfx_cli_opt_t* opt = opts;
    while (opts) {
        if (opts->short_name) {
            printf("short name: %c,", opts->short_name);
        }
        if (cf_string_len(&opts->long_name)) {
            printf("long name: %s,", cf_string_ptr(&opts->long_name));
        }
        printf("flag: %d, required: %d, has_value: %d,", opts->flag, opts->required, opts->has_value);
        if (opts->has_value && !opts->flag) {
            printf("value: %s", opts->value.ptr);
        }
        printf("\n");
        opts = opts->next;
    }
    EXPECT_EQ(cfx_cli_opt_exist_sn(opt, 'a'), CF_TRUE);
    EXPECT_EQ(cfx_cli_opt_exist_ln(opt, "aaa"), CF_TRUE);
    EXPECT_EQ(cfx_cli_opt_exist_sn(opt, 's'), CF_FALSE);
    EXPECT_EQ(cfx_cli_opt_exist_ln(opt, "sss"), CF_FALSE);
    EXPECT_EQ(cfx_cli_opt_has_val_sn(opt, 'a'), CF_TRUE);
    EXPECT_EQ(cfx_cli_opt_has_val_sn(opt, 'd'), CF_TRUE);
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

    
    cfx_cli_opt_t opt_a;
    cfx_cli_opt_t opt_b;
    cfx_cli_opt_t opt_c;
    cfx_cli_opt_t opt_d;
    cfx_cli_opt_init(&opt_a, 'a', "aaa", "option a", CF_TRUE, CF_FALSE, "");
    cfx_cli_opt_init(&opt_b, 'b', "bbb", "option b", CF_TRUE, CF_FALSE, "");
    cfx_cli_opt_init(&opt_c, 'c', "", "option c", CF_TRUE, CF_TRUE, "");
    cfx_cli_opt_init(&opt_d, 'd', "ddd", "option d", CF_TRUE, CF_TRUE, "");


    cfx_cli_cmd_add_opt(&subcmd11, &opt_a);
    cfx_cli_cmd_add_opt(&subcmd11, &opt_b);
    cfx_cli_cmd_add_opt(&subcmd11, &opt_c);
    cfx_cli_cmd_add_opt(&subcmd11, &opt_d);

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
