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
    return 0;
}