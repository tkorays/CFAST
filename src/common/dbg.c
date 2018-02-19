#include <cf/dbg.h>
#include <cf/err.h>
#include <cf/str.h>

CF_DECLARE(cf_size_t) cf_dbg_mem2hex(cf_void_t* m, cf_size_t n, cf_char_t* buf, cf_size_t size) {
    cf_char_t* s = (cf_char_t*)m;
    cf_size_t wrt_cnt = 0, chr_cnt = 0, cnt = 0;
    if(!m || !buf || size <= (n<<1)) return 0;
    while(n > 0) {
        if(chr_cnt && (chr_cnt % 16 == 0)) {
            cnt = cf_snprintf(buf + wrt_cnt, size - wrt_cnt, "\n");
            if(cnt <= 0) return 0;
            wrt_cnt += cnt;
        }
        if((chr_cnt % 16 != 0) && (chr_cnt % 4 == 0)) {
            cnt = cf_snprintf(buf + wrt_cnt, size - wrt_cnt, " ");
            if(cnt <= 0) return 0;
            wrt_cnt += cnt;
        }
        cnt = cf_snprintf(buf + wrt_cnt, size - wrt_cnt, "%X", *s);
        if(cnt <= 0) return 0;
        wrt_cnt += cnt;
        
        s++;
        chr_cnt++;
        n--;
    }
    return cnt;
}