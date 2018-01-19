#include <cf/types.h>
#include <cf/config.h>


#define MAKE_VERSION(a, b) #a "." #b

const cf_char_t* CF_VERSION = MAKE_VERSION(CF_VERSION_NUM_MAJOR, CF_VERSION_NUM_MINOR);

const char* cf_get_version() {
    return CF_VERSION;
}
