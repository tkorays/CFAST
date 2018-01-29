#include <cfast.h>
#include <stdio.h>

cf_opt_t myopts[] = {
    {CF_OPT_ID_CMD_SELF, '-', "-", CF_OPT_OPTIONAL_ARG_OPTIONAL},
    {1, 'a', "abc", CF_OPT_ESSENTIAL_ARG_ESSENTIAL},
    {2, 'b', "bcd", CF_OPT_ESSENTIAL_ARG_OPTIONAL},
    {3, 'c', "cde", CF_OPT_OPTIONAL_ARG_ESSENTIAL},
    {4, 'd', "def", CF_OPT_OPTIONAL_ARG_OPTIONAL},
    {5, 'e', CF_NULL_PTR, CF_OPT_OPTIONAL_ARG_OPTIONAL},
    {CF_OPT_ID_INVALID, '0', CF_NULL_PTR, CF_OPT_OPTIONAL_ARG_OPTIONAL}
};

// xxx -a "fuck" -de --bcd --abc="wocao"

int main(int argc, char* argv[])
{
    cf_opt_it_t it = {0};
    cf_errno_t ret = CF_OK;
    printf("opt test\n");
    while(CF_OK == (ret = cf_opt_get(argc, argv, myopts, &it)))
    {
        printf("%u, name(%s), hasarg(%u)\n", it.opt->id, it.opt->lname, it.hasarg);
    }
    if(ret != CF_OK && ret != CF_EEOF) {
        printf("parse error! ret: %d\n", ret);
    }
    return 0;
}