#include "cfx/json.h"

#include <stdio.h>

const cf_char_t* json_str = "{\"a\": 123, \"b\": [123, 567, 789], \"c\": {\"a\": 123, \"b\": \"234\"}, \"d\": null}";

int main(int argc, char *argv[]) {
    cfx_json_t* json = cfx_json_new();
    cf_char_t buf[1024];
    buf[0] = '\0';
    cfx_json_parse(json, json_str);
    cfx_json_dump(json, buf, 1024);
    printf("json: %s\n", buf);

    cfx_json_delete(json);
    printf("hello world");
    return 0;
}
