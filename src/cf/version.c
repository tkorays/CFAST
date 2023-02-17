#include "cf/version.h"
#include "cf/str.h"
#include "cf/memory.h"
#include "cf/slice.h"


#define _IS_VALID_VER_CHAR(c) (CF_IS_DIGIT(c) || (c) == '.' || (c) == 'a' || (c) == 'b' || (c) == '\0')

cf_bool_t cf_version_from(cf_version_t* v, const char* t) {
    int num = 0;
    int versions[5] = {0};
    int version_idx = 0;
    cf_size_t size = 0;

    if (!v || !t) return CF_FALSE;

    while(CF_TRUE) {
        if (!_IS_VALID_VER_CHAR(*t)) return CF_FALSE; 

        if (version_idx >= 5) return CF_FALSE;

        if (*t == '.' || *t == '\0') {
            if (size == 0) return CF_FALSE;

            versions[version_idx] = num;
            num = 0;
            size = 0;
            version_idx++;
        }
        else if (*t == 'a' || *t == 'b') {
            // get prerelase tag again
            if (version_idx > 3) return CF_FALSE;

            if (size == 0) return CF_FALSE;

            versions[version_idx] = num;
            versions[3] = *t;

            num = 0;
            size = 0;
            version_idx = 4;

        } else {
            // don't support 1.2.2.3
            if (version_idx == 3) return CF_FALSE;
            num = num * 10 + (*t - '0');
            ++size;
        }

        if (*t == '\0') break;
        ++t;
    }
    v->major = versions[0];
    v->minor = versions[1];
    v->patch = versions[2];
    v->pretag = versions[3];
    v->prenum =  versions[4];

    return CF_TRUE;
}

int cf_version_compare(const cf_version_t* v1, const cf_version_t* v2) {
    if (!v1 && !v2) return 0; 
    if (!v1) return -1;
    if (!v2) return 1;
    if (v1->major != v2->major) {
        return v1->major > v2->major ? 1 : -1;
    }
    if (v1->minor != v2->minor) {
        return v1->minor > v2->minor ? 1 : -1;
    }
    if (v1->patch != v2->patch) {
        return v1->patch > v2->patch ? 1 : -1;
    }
    if (v1->pretag != v2->pretag) {
        return v1->pretag > v2->pretag ? 1 : -1;
    }
    if (v1->prenum != v2->prenum) {
        return v1->prenum > v2->prenum ? 1 : -1;
    }
    return 0;
}

cf_bool_t cf_version_eq(const cf_version_t* v1, const cf_version_t* v2) {
    return cf_version_compare(v1, v2) == 0;
}

cf_bool_t cf_version_gt(const cf_version_t* v1, const cf_version_t* v2) {
    return cf_version_compare(v1, v2) > 0;
}

cf_bool_t cf_version_lt(const cf_version_t* v1, const cf_version_t* v2) {
    return cf_version_compare(v1, v2) < 0;
}

cf_bool_t cf_version_ge(const cf_version_t* v1, const cf_version_t* v2) {
    return cf_version_compare(v1, v2) >= 0;
}

cf_bool_t cf_version_le(const cf_version_t* v1, const cf_version_t* v2) {
    return cf_version_compare(v1, v2) <= 0;
}

