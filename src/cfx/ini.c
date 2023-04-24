#include "cfx/ini.h"
#include "cf/memory.h"
#include "cf/array.h"

typedef struct {
    cf_char_t*  buff;       /** buffer for key/value pair */
    cf_size_t   offset;     /** offset for value */
} string_kv;

typedef struct {
    cf_char_t*  name;
    cf_array_t  kv;
} cfx_ini_section;

struct cfx_ini {
    cfx_ini_section global_section;
    cf_array_t      sections;
    cf_char_t       path;
};

cfx_ini_t* cfx_ini_new() {
    cfx_ini_t* ini = cf_malloc_z(sizeof(cfx_ini_t));
    return ini;
}

cf_void_t cfx_ini_delete(cfx_ini_t* self) {
    if (self) cf_free(self);
}

cf_bool_t cfx_ini_parse(cfx_ini_t* self, const cf_char_t* file) {
    return CF_TRUE;
}

cf_bool_t cfx_ini_save(cfx_ini_t* self, const cf_char_t* file) {
    return CF_TRUE;
}


cf_void_t cfx_ini_set(cfx_ini_t* self,
                      const cf_char_t* section,
                      const cf_char_t* key,
                      const cf_char_t* value) {

}

cf_bool_t cf_ini_get(cfx_ini_t* self,
                     const cf_char_t* section,
                     const cf_char_t* key,
                     cf_char_t* buffer,
                     cf_size_t size) {
    return CF_TRUE;
}