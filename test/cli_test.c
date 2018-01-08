#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cfast/cf_cli_if.h>

cf_void_t _input_(cf_char_t* buf, cf_size_t bufsize) {
    if(!buf) return;
    printf(">");
    fgets(buf, bufsize, stdin);
    if (buf[strlen(buf)-1] == '\n')
        buf[strlen(buf)-1] = '\0';
}

cf_errno_t proc_cmd(cf_cli_cmd_cxt_t* cxt, cf_char_t* s) {
    printf("proc ahhaha");
    return 0;
}


cf_cli_cmd_t cmds[] = {
    {0, "abc", 0, 0, 0, 0, "help info", proc_cmd},
};

int main() {
    cf_cli_t* cli = cf_cli_init();
    cf_cli_install_all_cmds(cli, cmds);
    cf_cli_set_io_func(cli, _input_, 0);
    cf_cli_run(cli);
    cf_cli_deinit(cli);
    return 0;
}