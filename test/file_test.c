#include <cfast/cf_file_if.h>
#include <stdio.h>

int main() {
    struct cf_file_dir_s dir;
    struct cf_file_dirent_s dire;
    cf_file_opendir(&dir, ".");
    while(CF_OK == cf_file_readdir(&dir, &dire)) {
        printf("get file: %s\n", dire.name);
    }
    cf_file_closedir(&dir);

    printf("file log_test.c exist: %u\n", cf_file_exist("log_test.c"));
    printf("file log_test.ca not exist: %u\n", cf_file_exist("log_test.ca"));
    return 0;
}