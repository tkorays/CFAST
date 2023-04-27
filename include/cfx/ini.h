#ifndef __CFX_INI_H__
#define __CFX_INI_H__

#include "cf/types.h"

CF_DECLS_BEGIN

/**
 * INI reader/writer.
 */
typedef struct cfx_ini cfx_ini_t;

/**
 * create a ini reader/writer
 * @return the created object
 */
cfx_ini_t* cfx_ini_new();

/**
 * delete a ini reader/writer
 * @param self the object to delete
 */
cf_void_t cfx_ini_delete(cfx_ini_t* self);

/**
 * parse a ini file
 * @param self the ini reader/writer
 * @param file the file to parse
 * @return true if successful, false otherwise
 */
cf_bool_t cfx_ini_parse(cfx_ini_t* self, const cf_char_t* file);

/**
 * save a ini file
 * @param self the ini reader/writer
 * @param file the file to save
 * @return true if successful, false otherwise
 */
cf_bool_t cfx_ini_save(cfx_ini_t* self, const cf_char_t* file);

/**
 * input a line to the ini reader/writer
 * @param self the ini reader/writer
 * @param line the line to input
 * @return true if successful, false otherwise
 */
cf_bool_t cfx_ini_input(cfx_ini_t* self, const cf_char_t* line);

/**
 * set a value in the ini reader/writer
 * @param self the ini reader/writer
 * @param section the section to set the value in
 * @param key the key to set the value for
 * @param value the value to set
 * @return true if successful, false otherwise
 */
cf_bool_t cfx_ini_set(cfx_ini_t* self,
                      const cf_char_t* section,
                      const cf_char_t* key,
                      const cf_char_t* value);

/**
 * get a value from the ini reader/writer
 * @param self the ini reader/writer
 * @param section the section to get the value from
 * @param key the key to get the value for
 * @param buffer the buffer to store the value in
 * @param size the size of the buffer
 * @return true if successful, false otherwise
 */
cf_bool_t cfx_ini_get(cfx_ini_t* self,
                     const cf_char_t* section,
                     const cf_char_t* key,
                     cf_char_t* buffer,
                     cf_size_t size);


CF_DECLS_END

#endif /* __CFX_INI_H__ */
