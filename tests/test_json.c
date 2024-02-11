#include "cfx/json.h"

#include <stdio.h>

const cf_char_t* json_str = "{\"a\": 123, \"b\": [123, 567, 789], \"c\": {\"a\": 123, \"b\": \"234\"}, \"d\": null}";

int main(int argc, char *argv[]) {
    cfx_json_t* json = cfx_json_new();
    cfx_json_t* i1 = cfx_json_new_int(123);
    cfx_json_t* d1 = cfx_json_new_double(555.5);
    cfx_json_t* s1 = cfx_json_new_string("abc");
    cfx_json_t* s2 = cfx_json_new_string("efg");
    cfx_json_t* a1 = cfx_json_new_array();
    cfx_json_object_add(json, "i1", i1);
    cfx_json_object_add(json, "d1", d1);
    cfx_json_array_add(a1, s1);
    cfx_json_array_add(a1, s2);
    cfx_json_object_add(json, "a1", a1);
    cf_char_t buf[1024];
    cfx_json_dump(json, buf, 1024);
    printf("json: %s\n", buf);

    cfx_json_delete(json);
    return 0;
}
