/**
 * @file fifobuf.h
 * @brief Header file for the FIFO buffer implementation.
 * 
 * This file contains the definition of the FIFO buffer data structure and the
 * function prototypes for creating, deleting, and manipulating FIFO buffers.
 * 
 * @note This header file follows the same style as types.h.
 */


#ifndef __CF_FIFOBUF_H__
#define __CF_FIFOBUF_H__

#include "cf/types.h"

CF_DECLS_BEGIN

/**
 * @struct cf_fifobuf
 * @brief Structure representing a FIFO buffer.
 */
typedef struct cf_fifobuf cf_fifobuf_t;

/**
 * @brief Creates a new FIFO buffer.
 * @param nullable_buffer   A provided buffer. If the buffer is null, we'll create one.
 * @param size              The size of the buffer.
 * @return A pointer to the newly created FIFO buffer.
 */
cf_fifobuf_t* cf_fifobuf_new(cf_void_t* nullable_buffer, unsigned size);

/**
 * @brief Deletes a FIFO buffer.
 * @param self A pointer to the FIFO buffer to delete.
 * @return return the real buffer address if the buffer is not owned by fifo.
 * 
 * The invoker is responsible for destroying the provided buffer.
 */
cf_void_t* cf_fifobuf_delete(cf_fifobuf_t* self);

/**
 * @brief Returns the size of a FIFO buffer.
 * @param self A pointer to the FIFO buffer.
 * @return The size of the buffer.
 */
cf_size_t cf_fifobuf_size(cf_fifobuf_t* self);

/**
 * @brief Reads a specified number of bytes from a FIFO buffer.
 * @param self A pointer to the FIFO buffer.
 * @param buf  output buffer.
 * @param size Pass the input buffer size and output the read bytes.
 * @return true for success
 */
cf_bool_t cf_fifobuf_read(cf_fifobuf_t* self, cf_void_t* buf, cf_size_t* size);

/**
 * @brief Writes a buffer onto a FIFO buffer.
 * @param self A pointer to the FIFO buffer.
 * @param buf A pointer to the buffer to write.
 * @return true for success
 */
cf_bool_t cf_fifobuf_write(cf_fifobuf_t* self, cf_void_t* buf, cf_size_t size);

/**
 * @brief Reads a specified number of bytes from a FIFO buffer.
 * @param self A pointer to the FIFO buffer.
 * @param ptr  output buffer.
 * @param size Pass the input buffer size and output the read bytes.
 * @return true for success
 */
cf_bool_t cf_fifobuf_peek(cf_fifobuf_t* self, cf_void_t* buf, cf_size_t* size);

CF_DECLS_END

#endif /* __CF_FIFOBUF_H__ */
