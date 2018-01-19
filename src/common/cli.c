#include <cf/cli.h>
#include <cf/memory.h>
#include <cf/err.h>
#include <stdio.h>
#include <string.h>


cf_void_t _input_func(cf_char_t* buf, cf_size_t bufsize) {
    if(!buf || bufsize == 0) return ;
    fgets(buf, bufsize, stdin);
    if (buf[strlen(buf)-1] == '\n')
        buf[strlen(buf)-1] = '\0';
}

cf_void_t _output_func(cf_char_t* buf) {
    printf("%s\n", buf);
}

cf_cli_t* cf_cli_init() {
    cf_cli_t* cli = CF_NULL_PTR;
    cli = (cf_cli_t*)cf_malloc(sizeof(cf_cli_t));
    if(!cli) return CF_NULL_PTR;

    cli->input = _input_func;
    cli->output = _output_func;
    cli->root = CF_NULL_PTR;
    (cf_void_t)cf_memset_s(cli->inbuf, sizeof(cli->inbuf), 0, sizeof(cli->inbuf));
    return cli;
}

cf_void_t   cf_cli_deinit(cf_cli_t* cli) {
    if(cli) {
        cf_free(cli);
    }
}

cf_errno_t  cf_cli_set_io_func(cf_cli_t* cli, cf_cli_pfn_input input, cf_cli_pfn_output output) {
    if(!cli) return CF_EPARAM;

    if(input) {
        cli->input = input;
    }

    if(output) {
        cli->output = output;
    }
    return CF_OK;
}

cf_errno_t  cf_cli_install_all_cmds(cf_cli_t* cli, cf_cli_cmd_t* root) {
    if(!cli || !root) return CF_EPARAM;
    cli->root = root;
    return CF_OK;
}

cf_errno_t  cf_cli_run_line(cf_cli_t* cli, cf_char_t* line) {
    if(!cli || !line) return CF_EPARAM;
    // todo: just output
    cli->output(line);
    return CF_OK;
}

cf_errno_t cf_cli_run(cf_cli_t* cli) {
    if(!cli) return CF_EPARAM;
    while(CF_TRUE) {
        cli->input(cli->inbuf, sizeof(cli->inbuf));
        if(strcmp(cli->inbuf, "exit") == 0) break;
        cf_cli_run_line(cli, cli->inbuf);
    }
    return CF_OK;
}
