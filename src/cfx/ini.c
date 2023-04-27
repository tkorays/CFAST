#include "cfx/ini.h"
#include "cf/memory.h"
#include "cf/array.h"
#include "cf/file.h"
#include "cf/str.h"
#include "cf/hashtbl.h"
#include "cf/assert.h"
#include <stdio.h>

cf_void_t delete_value_cb(cf_void_t* value) {
    if (value) {
        cf_free(value);
    }
}

cf_void_t delete_section_cb(cf_void_t* value) {
    cf_hashtbl_t* tbl = CF_TYPE_CAST(cf_hashtbl_t*, value);
    cf_hashtbl_delete(tbl, delete_value_cb);
}

struct cfx_ini {
    cf_char_t           path[CF_MAX_PATH_SIZE];     /** the opened path */
    cf_char_t*          buffer;                     /** parse buffer */
    cf_size_t           buffer_size;                /** buffer size */
    cf_int_t            line;                       /** line */
    cf_hashtbl_t*       sections;                   /** sections */
    cf_hashtbl_t*       cur_section;                /** point to currenet section */
};

cfx_ini_t* cfx_ini_new() {
    cfx_ini_t* ini = cf_malloc_z(sizeof(cfx_ini_t));

    ini->sections = cf_hashtbl_new(8);
    ini->cur_section = cf_hashtbl_new(16);

    /* the first one is always the global section */
    cf_hashtbl_set_by_hash(ini->sections, 0, ini->cur_section);

    ini->buffer         = cf_malloc(1024);
    ini->buffer_size    = 1024;
    return ini;
}

cf_void_t cfx_ini_delete(cfx_ini_t* self) {
    if (self) {
        cf_hashtbl_delete(self->sections, delete_section_cb);
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
        cf_hashtbl_set(self->sections, L + 1, CF_HASH_STRING_KEY_LEN_AUTO, tbl);
        self->cur_section = tbl;
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
        cf_hashtbl_set(self->cur_section, L, CF_HASH_STRING_KEY_LEN_AUTO, value);
    }
    return CF_TRUE;
}

cf_bool_t cfx_ini_save(cfx_ini_t* self, const cf_char_t* file) {
    cf_file_t f = CF_NULL_PTR;
    cf_hashtbl_iter_t iter, iter2;
    cf_hashtbl_t* sect = CF_NULL_PTR;
    cf_char_t* key = CF_NULL_PTR;
    cf_char_t* value = CF_NULL_PTR;

    if (file) {
        if (CF_EOK != cf_file_open(&f, file, "w+")) {
            return CF_FALSE;
        }
    } else {
        if (CF_EOK != cf_file_open(&f, self->path, "w+")) {
            return CF_FALSE;
        }
    }
    
    cf_file_printf(f, "; generate by CFAST(tkorays)\n\n");

    iter = cf_hashtbl_iter_init(self->sections);
    while (!cf_hashtbl_iter_end(self->sections, iter)) {
        key = cf_hashtbl_iter_key(iter);
        value = cf_hashtbl_iter_value(iter);
        if (key != CF_NULL_PTR) {
            /* print section header */
            cf_file_printf(f, "[%s]\n", key);
        }

        cf_assert(value != CF_NULL_PTR);

        sect = CF_TYPE_CAST(cf_hashtbl_t*, value);
        iter2 = cf_hashtbl_iter_init(sect);
        while (!cf_hashtbl_iter_end(sect, iter2)) {
            key = cf_hashtbl_iter_key(iter2);
            value = cf_hashtbl_iter_value(iter2);
            cf_assert(key != CF_NULL_PTR);
            cf_assert(value != CF_NULL_PTR);

            cf_file_printf(f, "%s = %s\n", key, value);

            iter2 = cf_hashtbl_iter_next(sect, iter2);
        }
        iter = cf_hashtbl_iter_next(self->sections, iter);
    }

    cf_file_close(f);
    return CF_TRUE;
}


cf_bool_t cfx_ini_set(cfx_ini_t* self,
                      const cf_char_t* section,
                      const cf_char_t* key,
                      const cf_char_t* value) {
    cf_hashtbl_t* sect = CF_NULL_PTR;
    cf_char_t* new_value = CF_NULL_PTR;

    if (CF_NULL_PTR == key || CF_NULL_PTR == value) return CF_FALSE;
    if (key && cf_strlen(key) == 0) return CF_FALSE;

    if (section == CF_NULL_PTR || (section && cf_strlen(section) == 0)) {
        sect = cf_hashtbl_get_by_hash(self->sections, 0); 
        cf_assert(sect != CF_NULL_PTR);
    } else {
        sect = cf_hashtbl_get(self->sections, section, CF_HASH_STRING_KEY_LEN_AUTO);
    }
    if (!sect) {
        sect = cf_hashtbl_new(8);
        cf_hashtbl_set(self->sections, section, CF_HASH_STRING_KEY_LEN_AUTO, sect);
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
        sect = cf_hashtbl_get_by_hash(self->sections, 0);
    } else {
        sect = cf_hashtbl_get(self->sections, section, CF_HASH_STRING_KEY_LEN_AUTO);
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
