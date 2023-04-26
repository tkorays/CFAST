#ifndef __CFX_INI_H__
#define __CFX_INI_H__

#include "cf/types.h"

CF_DECLS_BEGIN

typedef struct cfx_ini cfx_ini_t;

cfx_ini_t* cfx_ini_new();

cf_void_t cfx_ini_delete(cfx_ini_t* self);

cf_bool_t cfx_ini_parse(cfx_ini_t* self, const cf_char_t* file);

cf_bool_t cfx_ini_save(cfx_ini_t* self, const cf_char_t* file);

cf_bool_t cfx_ini_input(cfx_ini_t* self, const cf_char_t* line);


cf_bool_t cfx_ini_set(cfx_ini_t* self,
                      const cf_char_t* section,
                      const cf_char_t* key,
                      const cf_char_t* value);

cf_bool_t cfx_ini_get(cfx_ini_t* self,
                     const cf_char_t* section,
                     const cf_char_t* key,
                     cf_char_t* buffer,
                     cf_size_t size);

CF_DECLS_END

#endif /* __CFX_INI_H__ */
