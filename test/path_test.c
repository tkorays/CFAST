#include <cfast.h>
#include <stdio.h>

int main() {
    cf_char_t *p1 = "/a/b/c/d.txt";
    cf_char_t *p2 = "";
    cf_char_t *p3 = "abc";
    cf_char_t *p4 = "/abc/efg/";

    cf_char_t p[256] = {0};
    cf_path_basename(p1, p, 256);
    printf("org: %s, out: %s\n", p1, p);
    cf_path_basename(p2, p, 256);
    printf("org: %s, out: %s\n", p2, p);
    cf_path_basename(p3, p, 256);
    printf("org: %s, out: %s\n", p3, p);
    cf_path_basename(p4, p, 256);
    printf("org: %s, out: %s\n", p4, p);

    cf_path_dirname(p1, p, 256);
    printf("org: %s, out: %s\n", p1, p);
    cf_path_dirname(p2, p, 256);
    printf("org: %s, out: %s\n", p2, p);
    cf_path_dirname(p3, p, 256);
    printf("org: %s, out: %s\n", p3, p);
    cf_path_dirname(p4, p, 256);
    printf("org: %s, out: %s\n", p4, p);
    

    printf("%d, %d, %d\n",
    cf_path_exist("./abc"),
    cf_path_exist("/"),
    cf_path_exist("."));

    printf("%d,%d,%d\n",
    cf_path_isfile("a.out"),
    cf_path_isdir("/"),
    cf_path_isfile("/"));

    printf("is abs: %d, %d, %d\n", cf_path_isabs("/abc/"),
        cf_path_isabs("../a"),
        cf_path_isabs("abc"));
    *p = 0;
    cf_path_realpath("../b/../c/abc", p, 256);
    printf("realpath: %s\n", p);

    if(CF_EOK != cf_path_realpath("/../b/../c//abc", p, 256)) {
        printf("realpath fail\n");
    }else {
        printf("realpath: %s\n", p);
    }
    return 0;
}