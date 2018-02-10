#include <cf/err.h>
#include <stdio.h>

// 定义一个错误模块id
#define CF_ERR_MODULE_SB 2

// 生成模块内错误号
#define CF_ESB_0 CF_MAKE_ERRNO(CF_ERR_MODULE_SB, 0)
#define CF_ESB_1 CF_MAKE_ERRNO(CF_ERR_MODULE_SB, 1)
#define CF_ESB_2 CF_MAKE_ERRNO(CF_ERR_MODULE_SB, 2)
#define CF_ESB_3 CF_MAKE_ERRNO(CF_ERR_MODULE_SB, 3)
#define CF_ESB_4 CF_MAKE_ERRNO(CF_ERR_MODULE_SB, 4)
#define CF_ESB_5 CF_MAKE_ERRNO(CF_ERR_MODULE_SB, 5)
#define CF_ESB_X CF_MAKE_ERRNO(CF_ERR_MODULE_SB, 1000)

// 模块错误对应的错误详情
cf_err_info_t sb_errstr[] = {
        CF_ERR_STR_BUILD(CF_ESB_0, "sb errno 0"),
        CF_ERR_STR_BUILD(CF_ESB_1, "sb errno 1"),
        CF_ERR_STR_BUILD(CF_ESB_2, "sb errno 2"),
        CF_ERR_STR_BUILD(CF_ESB_3, "sb errno 3"),
        CF_ERR_STR_BUILD(CF_ESB_4, "sb errno 4"),
        CF_ERR_STR_BUILD(CF_ESB_5, "sb errno 5"),
        CF_ERR_STR_BUILD(CF_ESB_X, "sb err id NOT FOUND!")
};

// 获取模块错误吗的函数
const cf_char_t* sb_strerror(cf_uint_t eid) {
    cf_uint_t i;
    for (i = 0; i < CF_ARRAY_SIZE(sb_errstr); ++i) {
        if(eid == CF_ERR_GET_MERR(sb_errstr[i].err)) return sb_errstr[i].desc;
    }
    return sb_errstr[CF_ARRAY_SIZE(sb_errstr) - 1].desc;
}

int main() {
    cf_char_t buff[CF_ERR_DESC_MAX_BUFF];

    // 注册错误码到错误模块
    cf_err_register(CF_ERR_MODULE_SB, sb_strerror);

    // 这时候可以直接使用模块的错误函数获取错误码
    cf_err_strerr(CF_ESB_5, buff, CF_ERR_DESC_MAX_BUFF);
    printf("%s\n", buff);

    cf_err_strerr(CF_ESB_5 + 1, buff, CF_ERR_DESC_MAX_BUFF);
    printf("%s\n", buff);

    cf_err_strerr(CF_EFOPEN, buff, CF_ERR_DESC_MAX_BUFF);
    printf("%s\n", buff);

    cf_err_strerr(CF_EFOPEN + 6, buff, CF_ERR_DESC_MAX_BUFF);
    printf("%s\n", buff);

    return 0;
}