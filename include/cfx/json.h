#ifndef __CFX_JSON_H__
#define __CFX_JSON_H__

#include "cf/types.h"


CF_DECLS_BEGIN

/**
 * types for json value
 */
typedef enum cfx_json_type {
    CFX_JSON_VALUE_TYPE_NUMBER,
    CFX_JSON_VALUE_TYPE_STRING,
    CFX_JSON_VALUE_TYPE_BOOLEAN,
    CFX_JSON_VALUE_TYPE_OBJECT,
    CFX_JSON_VALUE_TYPE_ARRAY,
    CFX_JSON_VALUE_TYPE_NULL,
} CFX_JSON_VALUE_TYPE;

/**
 * json object
 */
typedef struct cfx_json {
    CFX_JSON_VALUE_TYPE     type;
    cf_char_t*              name;
    union {
        double              number;
        cf_char_t*          string;
        cf_bool_t           boolean;
        struct cfx_json*    child;
    } value;
    struct cfx_json*        next;
} cfx_json_t;


cfx_json_t* cfx_json_new();

void cfx_json_delete(cfx_json_t* self);

cfx_json_t* cfx_json_load(const cf_char_t* file);

cf_bool_t cfx_json_dump(cfx_json_t* self, const cf_char_t* file);

cfx_json_t* cfx_json_load_s(const cf_char_t* json_str);

cf_size_t cfx_json_dump_s(cfx_json_t* self, cf_char_t* buf, cf_size_t size);

cfx_json_t* cfx_json_new_int(int value);
cfx_json_t* cfx_json_new_double(double value);
cfx_json_t* cfx_json_new_string(const cf_char_t* value);
cfx_json_t* cfx_json_new_null();
cfx_json_t* cfx_json_new_bool(cf_bool_t value);
cfx_json_t* cfx_json_new_array();
cfx_json_t* cfx_json_new_object();
cfx_json_t* cfx_json_array_add(cfx_json_t* root, cfx_json_t* node);
cfx_json_t* cfx_json_object_add(cfx_json_t* root, const cf_char_t* name, cfx_json_t* node);

cfx_json_t* cfx_json_get(cfx_json_t* self, const cf_char_t* name);
cfx_json_t* cfx_json_get_cascade(cfx_json_t* self, const cf_char_t* name);

CF_DECLS_END


#endif /* __CFX_JSON_H__ */

