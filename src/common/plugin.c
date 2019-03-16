#include <cf/plugin.h>

CF_DECLARE(cf_errno_t) cf_plugin_initialize() {
    return CF_OK;
}

CF_DECLARE(cf_errno_t) cf_plugin_shutdown() {
    return CF_OK;
}

CF_DECLARE(cf_errno_t) cf_plugin_load(const cf_char_t* path, cf_plugin_context_t* plg) {
    return CF_OK;
}

CF_DECLARE(cf_errno_t) cf_plugin_unload(cf_plugin_context_t* plg) {
    return CF_OK;
}

CF_DECLARE(cf_errno_t) cf_plugin_version_str(const cf_plugin_context_t* plg, cf_char_t* buff, cf_size_t len) {
    return CF_OK;
}


