#ifndef __CF_OPT_H__
#define __CF_OPT_H__

#include <cf/types.h>

/**
 * defgroup CF_OPT opt
 * @{
 */

CF_DECLS_BEGIN

#define CF_OPT_NAME_MAX_SIZE 64
#define CF_OPT_ARG_MAX_SIZE 128
#define CF_OPT_MAX_SIZE 128


/**
 * @brief Structure representing a command line option
 * 
 * This structure contains information about a command line option, including
 * whether it is a multi-option, whether it is a short option, its name, whether
 * it has an argument, and the argument itself.
 */
typedef struct {
    cf_bool_t is_multi; /**< Whether this is a multi-option */
    cf_bool_t is_short; /**< Whether this is a short option */
    cf_char_t name[CF_OPT_NAME_MAX_SIZE]; /**< The name of the option */
    cf_bool_t has_arg; /**< Whether this option has an argument */
    cf_char_t arg[CF_OPT_ARG_MAX_SIZE]; /**< The argument for this option */
} cf_opt_t;

/**
 * @brief Get the raw command line option
 * 
 * This function retrieves the raw command line option from the given arguments.
 * It returns the option in the cf_opt_t structure.
 * 
 * @param argc The number of arguments
 * @param argv The array of arguments
 * @param pos The position of the option in the array
 * @param opt The cf_opt_t structure to store the option
 * @return cf_errno_t The error code
 */
cf_errno_t cf_opt_get_raw(cf_uint_t argc, cf_char_t* argv[], cf_uint_t* pos, cf_opt_t* opt);

CF_DECLS_END

/** @} */

#endif /* __CF_OPT_H__ */
