#include <cfast/cf_file_if.h>
#include <stdio.h>

int main() {
    cf_file_dir_t* dir = CF_NULL_PTR;
    cf_file_dirent_t *pd = CF_NULL_PTR;
    cf_file_opendir(dir, ".");
    while(CF_OK = cf_file_readdir(dir, pd)) {
        printf("get file.\n");
    }
    cf_file_closedir(dir);
    return 0;
}