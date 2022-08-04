#ifndef __CFAST_LOCATION_H__
#define __CFAST_LOCATION_H__

#include <cf/types.h>

CF_DECLS_BEGIN

typedef struct {
    char*   file_name;      //< filename of this location
    char*   function_name;  //< function name of this location
    int     line_number;    //< line number of this location
} cf_location_t;

/**
 * @brief define a location.
 * 
 */
#define cf_location_define(location) cf_location_t location;\
    do { (location).file_name = __FILE__; \
        (location).function_name = __FUNCTION__; (location).line_number = __LINE__; } while(0)

CF_DECLS_END


#endif /* __CFAST_LOCATION_H__ */
