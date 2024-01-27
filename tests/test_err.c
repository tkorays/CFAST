#include "cf/err.h"
#include "cf/assert.h"
#include "cf/str.h"


#define MY_ERR_MID 3
#define MY_ERR_1 CF_MAKE_ERRNO(MY_ERR_MID, 0)
#define MY_ERR_2 CF_MAKE_ERRNO(MY_ERR_MID, 2)

const cf_char_t* my_strerr_fn(cf_errno_t eid) {
    switch (eid) {
    case MY_ERR_1:
        return "err1";
    case MY_ERR_2:
        return "err2";
    default:
        return "Unkown Error!";
    }
}

int main(int argc, char *argv[]) {
    cf_char_t s[128];

    /* a bad module id, or a bad pointer */
    cf_assert(cf_err_register(CF_ERR_MID_NUM_MAX, my_strerr_fn) != CF_EOK);
    cf_assert(cf_err_register(1, CF_NULL_PTR) != CF_EOK);

    cf_err_register(MY_ERR_MID, my_strerr_fn);

    cf_err_strerr(MY_ERR_1, s, 128);
    cf_assert(cf_strcmp(s, "err1") == 0);
    cf_err_strerr(MY_ERR_2, s, 128);
    cf_assert(cf_strcmp(s, "err2") == 0);
    return 0;
}
