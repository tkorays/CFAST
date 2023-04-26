#include "cfx/ini.h"
#include "cf/memory.h"
#include "cf/array.h"
#include "cf/file.h"
#include "cf/str.h"
#include "cf/hashtbl.h"
#include "cf/mpool.h"
#include <stdio.h>


struct cfx_ini {
    cf_char_t           path[CF_MAX_PATH_SIZE];     /** the opened path */
    cf_char_t*          buffer;                     /** parse buffer */
    cf_size_t           buffer_size;                /** buffer size */
    cf_int_t            line;                       /** line */
    cf_hashtbl_t*       sect;                       /** sections */
    cf_hashtbl_t*       cur_sect;
};

cfx_ini_t* cfx_ini_new() {
    cfx_ini_t* ini = cf_malloc_z(sizeof(cfx_ini_t));

    ini->sect = cf_hashtbl_new(8);
    ini->cur_sect = cf_hashtbl_new(16);
    cf_hashtbl_set(ini->sect, "@", CF_HASH_STRING_KEY_LEN_AUTO, ini->cur_sect);

    ini->buffer         = cf_malloc(1024);
    ini->buffer_size    = 1024;
    return ini;
}

cf_void_t cfx_ini_delete(cfx_ini_t* self) {
    if (self) {
        /* TODO: destroy the hashtbl */
        if (self->buffer) {
            cf_free(self->buffer);
        }
        cf_free(self);
    }
}

cf_bool_t cfx_ini_parse(cfx_ini_t* self, const cf_char_t* file) {
    cf_file_t f = CF_NULL_PTR;

    if (CF_EOK != cf_file_open(&f, file, "r")) {
        return CF_FALSE;
    }

    while (!cf_file_eof(f)) {
        if (CF_OK != cf_file_gets(f, self->buffer, self->buffer_size)) {
            break;
        }
        
        if (!cfx_ini_input(self, self->buffer)) {
            printf("ERR: %s\n", self->buffer);
        }
    }

    cf_file_close(f);
    return CF_TRUE;
}

cf_bool_t cfx_ini_input(cfx_ini_t* self, const cf_char_t* line) {
    cf_char_t* L, *L1;
    cf_size_t len;
    cf_hashtbl_t* tbl = CF_NULL_PTR;
    cf_char_t* value;

    printf("> %s\n", line);
    self->line++;
    if (!cf_strcpy_s(self->buffer, self->buffer_size, line)) {
        cf_snprintf(self->buffer, self->buffer_size, "ERR: failed to copy line to buffer");
        return CF_FALSE;
    }
    if (!cf_str_strip(self->buffer)) {
        cf_snprintf(self->buffer, self->buffer_size, "ERR: failed to strip string");
        return CF_FALSE;
    }
    L = self->buffer;

    /* this line is a comment */
    if (L[0] == ';') {
        return CF_TRUE;
    } if (L[0] == '\0') {
        return CF_TRUE;
    }else if (L[0] == '[') {
        /* bad section */
        L1 = cf_strrchr(L, ']');
        if (!L1) {
            cf_snprintf(self->buffer, self->buffer_size, "ERR: bad section format, expect ']', line: %d!", self->line);
            return CF_FALSE;
        }
        *L1 = '\0';
        len = CF_TYPE_CAST(cf_uintptr_t, L1) - CF_TYPE_CAST(cf_uintptr_t, L);
        if (len <= 1) {
            cf_snprintf(self->buffer, self->buffer_size, "ERR: section name's length is zero, line: %d", self->line);
            return CF_FALSE;
        }

        tbl = cf_hashtbl_new(8);
        cf_hashtbl_set(self->sect, L + 1, CF_HASH_STRING_KEY_LEN_AUTO, tbl);
        self->cur_sect = tbl;
    } else if (L[0] == '=') {
        cf_snprintf(self->buffer, self->buffer_size, "ERR: ini key is NULL!");
        return CF_FALSE;
    } else {
        L1 = cf_strchr(L, '=');
        if (!L1) {
            cf_snprintf(self->buffer, self->buffer_size, "ERR: expect format: key = value, line: %d", self->line);
            return CF_FALSE;
        }
        *L1 = '\0';
        L1++;

        cf_str_strip(L);
        cf_str_strip(L1);

        len = cf_strlen(L);
        if (len == 0) {
            /* there is no need to check again */
            cf_snprintf(self->buffer, self->buffer_size, "ERR: ini key is NULL, line: %d", self->line);
            return CF_FALSE;
        }

        len = cf_strlen(L1);
        value = cf_malloc_z(len + 1);
        cf_strcpy_s(value, len + 1, L1);
        cf_hashtbl_set(self->cur_sect, L, CF_HASH_STRING_KEY_LEN_AUTO, value);
    }
    return CF_TRUE;
}

cf_bool_t cfx_ini_save(cfx_ini_t* self, const cf_char_t* file) {
    cf_file_t f = CF_NULL_PTR;

    if (file) {
        if (CF_EOK != cf_file_open(&f, file, "r")) {
            return CF_FALSE;
        }
    } else {
        if (CF_EOK != cf_file_open(&f, self->path, "r")) {
            return CF_FALSE;
        }
    }
    
    cf_file_printf(f, "; generate by CFAST\n");

    /** interate hashtbl */

    cf_file_close(f);
    return CF_TRUE;
}


cf_bool_t cfx_ini_set(cfx_ini_t* self,
                      const cf_char_t* section,
                      const cf_char_t* key,
                      const cf_char_t* value) {
    cf_hashtbl_t* sect = CF_NULL_PTR;
    cf_char_t* new_value = CF_NULL_PTR;
    if (section == CF_NULL_PTR || (section && cf_strlen(section) == 0)) {
        sect = cf_hashtbl_get(self->sect, "@", CF_HASH_STRING_KEY_LEN_AUTO);
    } else {
        sect = cf_hashtbl_get(self->sect, section, CF_HASH_STRING_KEY_LEN_AUTO);
    }
    if (!sect) {
        return CF_FALSE;
    }

    new_value = cf_malloc(cf_strlen(value) + 1);
    cf_strcpy_s(new_value, cf_strlen(value) + 1, value);
    cf_hashtbl_set(sect, key, CF_HASH_STRING_KEY_LEN_AUTO, new_value);

    return CF_TRUE;
}

cf_bool_t cfx_ini_get(cfx_ini_t* self,
                     const cf_char_t* section,
                     const cf_char_t* key,
                     cf_char_t* buffer,
                     cf_size_t size) {
    cf_hashtbl_t* sect = CF_NULL_PTR;
    cf_char_t* value = CF_NULL_PTR;
    if (section == CF_NULL_PTR || (section && cf_strlen(section) == 0)) {
        sect = cf_hashtbl_get(self->sect, "@", CF_HASH_STRING_KEY_LEN_AUTO);
    } else {
        sect = cf_hashtbl_get(self->sect, section, CF_HASH_STRING_KEY_LEN_AUTO);
    }
    if (!sect) {
        return CF_FALSE;
    }

    value = cf_hashtbl_get(sect, key, CF_HASH_STRING_KEY_LEN_AUTO);
    if (!value) {
        return CF_FALSE;
    }

    cf_strcpy_s(buffer, size, value);
    
    return CF_TRUE;
}