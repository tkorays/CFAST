#ifndef __CF_PLUGIN_H__
#define __CF_PLUGIN_H__

#include <cf/types.h>

#define CF_PLUGIN_NAME_MAX  128
#define CF_EPLG_NOT_INIT    CF_MAKE_ERRNO(CF_ERR_MODULE_PLUGIN, 0)
#define CF_EPLG_LOAD_FAIL   CF_MAKE_ERRNO(CF_ERR_MODULE_PLUGIN, 1)
#define CF_EPLG_UNLOAD_FAIL CF_MAKE_ERRNO(CF_ERR_MODULE_PLUGIN, 2)

typedef struct cf_plugin_context {
    const cf_char_t*    name;
    const cf_char_t*    description;
    const cf_char_t*    author;
    const cf_char_t*    email;
    cf_uint8_t          version_major;
    cf_uint8_t          version_minor;
    cf_uint16_t         size;
    cf_void_t*          objs;
    cf_void_t*          handle;
} cf_plugin_context_t;

CF_DECLARE(cf_errno_t) cf_plugin_initialize();
CF_DECLARE(cf_errno_t) cf_plugin_shutdown();
CF_DECLARE(cf_errno_t) cf_plugin_load(const cf_char_t* path, cf_plugin_context_t* plg);
CF_DECLARE(cf_errno_t) cf_plugin_unload(cf_plugin_context_t* plg);
CF_DECLARE(cf_errno_t) cf_plugin_version_str(const cf_plugin_context_t* plg, cf_char_t* buff, cf_size_t len);

#endif /* __CF_PLUGIN_H__ */
