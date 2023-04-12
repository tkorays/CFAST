#ifndef __CFX_ASCII_ART_H__
#define __CFX_ASCII_ART_H__

#include "cf/types.h"

CF_DECLS_BEGIN

/**
 * font used in cfx_art.
 */
typedef enum {
    CFX_ART_FONT_DEF,       /**  the default font */
    CFX_ART_FONT_SLANT,     /** slant font */
} cfx_art_font_t;

/**
 * @brief Prints the given ASCII art input to the given buffer.
 * 
 * @param input The ASCII art input to print.
 * @param buff The buffer to print the ASCII art to.
 * @param size The size of the buffer.
 * @param font font of the output ASCII.
 * @return cf_int_t write bytes count.
 */
cf_int_t cfx_art_print(const char* input, cf_char_t* buff, cf_size_t size, cfx_art_font_t font);

CF_DECLS_END

#endif /* __CFX_ASCII_ART_H__ */
