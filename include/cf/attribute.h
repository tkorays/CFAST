#ifndef __CF_ATTRIBUTE_H__
#define __CF_ATTRIBUTE_H__

#include "cf/types.h"
#include "cf/value.h"

CF_DECLS_BEGIN

#define CF_ATTRIBUTE_NONE   (0)

/**
 * Definition for an attribute object.
 * An attribute object contains an identifier and a value.
 * The identifier is used to identify the attribute, while the value contains the attribute's value.
 */
typedef struct {
    cf_uint32_t     id;     /** identifier for this attribute */
    cf_value_t      value;  /** attribute value */
} cf_attribute_t;

/**
 * @brief Initializes an attribute object.
 * 
 * @param attr The attribute object to initialize.
 */
CF_FORCE_INLINE cf_void_t cf_attribute_init(cf_attribute_t* attr) {
    attr->id = CF_ATTRIBUTE_NONE;
    cf_value_init(&attr->value);
}

/**
 * @brief Deinitializes an attribute object.
 * 
 * @param attr The attribute object to deinitialize.
 */
CF_FORCE_INLINE cf_void_t cf_attribute_deinit(cf_attribute_t* attr) {
    attr->id = CF_ATTRIBUTE_NONE;
    cf_value_deinit(&attr->value);
}

/**
 * @brief Returns a pointer to the value of an attribute object.
 * 
 * @param attr The attribute object to get the value from.
 * @return cf_value_t* A pointer to the value of the attribute object.
 */
CF_FORCE_INLINE cf_value_t* cf_attribute_value(cf_attribute_t* attr) {
    return &attr->value;
}


CF_DECLS_END

#endif /** __CF_ATTRIBUTE_H__ */
