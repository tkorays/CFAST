#include "cfx/json.h"

#include "cf/memory.h"
#include "cf/str.h"
#include "cf/file.h"
#include <stdio.h>

typedef struct string_stream {
    const cf_char_t*  str;
    cf_size_t   read;
} string_stream_t;

typedef cf_bool_t(*stream_read_func)(void* stream, cf_char_t* buff, cf_size_t size);


typedef struct {
    cf_char_t*  buffer;         // read buffer
    cf_size_t   buffer_size;    // read buffer size
    cf_size_t   buffer_idx;
    cf_char_t*  stack;
    cf_size_t   stack_size;
    cf_size_t   stack_idx;
    cf_int_t    line;
    cfx_json_t* output;

    void*       stream;
    stream_read_func read;
} json_parse_context;

void json_parse_context_init(json_parse_context* self, void* stream, stream_read_func f) {
    if (!self) {
        return;
    }
    cf_membzero(self, sizeof(json_parse_context));

    self->buffer = cf_malloc(1024);
    self->buffer_size = 1024;
    self->stack = cf_malloc_z(1024);
    self->stack_size = 1024;
    self->stream = stream;
    self->read = f;
}

cfx_json_t* json_parse_context_deinit(json_parse_context* self) {
    if (self && self->buffer) {
        cf_free(self->buffer);
    }
    if (self && self->stack) {
        cf_free(self->stack);
    }
    return self->output;
}

cf_bool_t json_parse_check_buffer(json_parse_context* self) {
    // buffer is empty or need a new line
    if (self->buffer_size == 0 || self->buffer[self->buffer_idx] == '\0') {
        if (!self->read(self->stream, self->buffer, self->buffer_size)) {
            return CF_FALSE;
        }
        self->buffer_idx = 0;
    }
    return CF_TRUE;
}

void json_parse_escape(json_parse_context* self) {
    cf_char_t c;
    if (!json_parse_check_buffer(self)) {
        return;
    }

    c = self->buffer[self->buffer_idx];
    while (CF_IS_SPACE(c) || c == '\0') {
        self->buffer_idx++;
        c = self->buffer[self->buffer_idx];
        if (!json_parse_check_buffer(self)) {
            return;
        }
    }
}
// parse-value
// [ -> parse-array until ]
// { -> parse-object until }
//      parse-string parse-comma parse-value
// " -> parse string until "
// parse-number/null
// parse-object
// parse-string
// parse-array

int json_parse_string(json_parse_context* self) {
    printf("parse string\n");
    return 0;
}

int json_parse_object(json_parse_context* self) {
    printf("parse object\n");
    if (self->buffer[self->buffer_idx] != '{') {
        return 0;
    }
    return 0;
}

int json_parse_array(json_parse_context* self) {
    printf("parse array\n");
    return 0;
}

void json_parse_value(json_parse_context* self) {
    cf_char_t* line = self->buffer;
    while (*line != '\0' || *line != '\r' || *line != '\n') {
        if (CF_IS_BLANK(*line)) {
            line++;
            continue;
        }

        if (*line == '{') {
            line += json_parse_object(self);
        } else if (*line == '[') {
            line += json_parse_array(self);
        }else if (*line == '"') {
            line += json_parse_string(self);
        } else {
            // parse number/null 
        } 
    }
}

void json_parse_context_run(json_parse_context* self) {
    /* escape blanks */
    json_parse_escape(self);
    printf("-- start -->\n");

    if (self->buffer[self->buffer_idx]== '{') {
        json_parse_object(self);
    } else if (self->buffer[self->buffer_idx] == '[') {
        json_parse_array(self);
    } else {
        json_parse_value(self);
    }
    json_parse_escape(self);
    // check remainings
}


cfx_json_t* cfx_json_new() {
    return cfx_json_new_object();
}

void cfx_json_delete(cfx_json_t* self) {
    cfx_json_t* first = CF_NULL_PTR;
    if (self->type == CFX_JSON_VALUE_TYPE_OBJECT ||
        self->type == CFX_JSON_VALUE_TYPE_ARRAY) {
        first = self->value.child;
        while (first) {
            // use child to cache the next
            self->value.child = first->next;
            cfx_json_delete(first);
            first = self->value.child; 
        }
    } else if (self->type == CFX_JSON_VALUE_TYPE_STRING) {
        if (self->value.string) {
            cf_free(self->value.string);
        }
    } 

    if (self->name) {
        cf_free(self->name);
    }
    cf_free(self);
}

cf_bool_t file_stream_read(void* stream, cf_char_t* buff, cf_size_t size) {
    return cf_file_gets(CF_TYPE_CAST(cf_file_t, stream), buff, size) == CF_OK;
}

cfx_json_t* cfx_json_load(const cf_char_t* file) {
    cf_file_t f;
    json_parse_context ctx;

    if (CF_EOK != cf_file_open(&f, file, "r")) {
        return CF_FALSE;
    }
    json_parse_context_init(&ctx, f, file_stream_read);
    json_parse_context_run(&ctx);

    cf_file_close(f);

    return json_parse_context_deinit(&ctx);
}

cf_bool_t string_stream_read(void* stream, cf_char_t* buff, cf_size_t size) {
    string_stream_t* s = CF_TYPE_CAST(string_stream_t*, stream);
    size_t i = 0;
    while (s->str[s->read] != '\0' && i < size) {
        buff[i++] = s->str[s->read++];
    }
    if (s->str[s->read] == '\0') {
        return CF_FALSE;
    }

    return CF_TRUE;
}

cfx_json_t* cfx_json_load_s(const cf_char_t* json_str) {
    json_parse_context ctx;
    string_stream_t s;
    s.str = json_str;
    s.read = 0;

    json_parse_context_init(&ctx, &s, string_stream_read);
    json_parse_context_run(&ctx);

    return json_parse_context_deinit(&ctx);
}

cf_bool_t cfx_json_dump(cfx_json_t* self, const cf_char_t* file) {
    return CF_TRUE;
}


cf_size_t cfx_json_dump_s(cfx_json_t* self, cf_char_t* buf, cf_size_t size) {
    cf_size_t writed_size = 0;
    cfx_json_t* child = CF_NULL_PTR;
    int cnt = 0;

    if (self->type == CFX_JSON_VALUE_TYPE_NUMBER) {
        writed_size += cf_snprintf(buf + writed_size, size - writed_size, "%f", self->value.number);
    } else if (self->type == CFX_JSON_VALUE_TYPE_STRING) {
        writed_size += cf_snprintf(buf + writed_size, size - writed_size, "\"%s\"", self->value.string);
    } else if (self->type == CFX_JSON_VALUE_TYPE_BOOLEAN) {
        writed_size += cf_snprintf(buf + writed_size, size - writed_size, "%s", self->value.boolean ? "true" : "false");
    } else if (self->type == CFX_JSON_VALUE_TYPE_NULL) {
        writed_size += cf_snprintf(buf + writed_size, size - writed_size, "null");
    } else if (self->type == CFX_JSON_VALUE_TYPE_OBJECT) {
        writed_size += cf_snprintf(buf + writed_size, size - writed_size, "{");
        child = self->value.child;
        while (child) {
            writed_size += cf_snprintf(buf + writed_size, size - writed_size, "\"%s\":", child->name);
            writed_size += cfx_json_dump_s(child, buf + writed_size, size - writed_size);
            child = child->next;
            if (child) {
                writed_size += cf_snprintf(buf + writed_size, size - writed_size, ",");
            }
        }

        writed_size += cf_snprintf(buf + writed_size, size - writed_size, "}");
    } else if (self->type == CFX_JSON_VALUE_TYPE_ARRAY) {
        writed_size += cf_snprintf(buf + writed_size, size - writed_size, "[");
        child = self->value.child;
        while (child) {
            writed_size += cfx_json_dump_s(child, buf + writed_size, size - writed_size);
            child = child->next;
            if (child) {
                writed_size += cf_snprintf(buf + writed_size, size - writed_size, ",");
            }
        }

        writed_size += cf_snprintf(buf + writed_size, size - writed_size, "]");
    }
    return writed_size;
}

cfx_json_t* cfx_json_new_int(int value) {
    cfx_json_t* json = cf_malloc_z(sizeof(cfx_json_t));
    json->type = CFX_JSON_VALUE_TYPE_NUMBER;
    json->value.number = value;
    return json;
}

cfx_json_t* cfx_json_new_double(double value) {
    cfx_json_t* json = cf_malloc_z(sizeof(cfx_json_t));
    json->type = CFX_JSON_VALUE_TYPE_NUMBER;
    json->value.number = value;
    return json;
}

cfx_json_t* cfx_json_new_string(const cf_char_t* value) {
    cfx_json_t* json = cf_malloc_z(sizeof(cfx_json_t));
    json->type = CFX_JSON_VALUE_TYPE_STRING;
    json->value.string = cf_malloc(cf_strlen(value) + 1);
    cf_strcpy_s(json->value.string, cf_strlen(value) + 1, value);
    return json;
}

cfx_json_t* cfx_json_new_null() {
    cfx_json_t* json = cf_malloc_z(sizeof(cfx_json_t));
    json->type = CFX_JSON_VALUE_TYPE_NULL;
    return json;
}

cfx_json_t* cfx_json_new_bool(cf_bool_t value) {
    cfx_json_t* json = cf_malloc_z(sizeof(cfx_json_t));
    json->type = CFX_JSON_VALUE_TYPE_BOOLEAN;
    json->value.boolean = value;
    return json;
}

cfx_json_t* cfx_json_new_array() {
    cfx_json_t* json = cf_malloc_z(sizeof(cfx_json_t));
    json->type = CFX_JSON_VALUE_TYPE_ARRAY;
    return json;
}

cfx_json_t* cfx_json_new_object() {
    cfx_json_t* json = cf_malloc_z(sizeof(cfx_json_t));
    json->type = CFX_JSON_VALUE_TYPE_OBJECT;
    return json;
}

cfx_json_t* cfx_json_array_add(cfx_json_t* root, cfx_json_t* node) {
    cfx_json_t* tmp = CF_NULL_PTR;
    if (root->type != CFX_JSON_VALUE_TYPE_ARRAY) {
        return CF_NULL_PTR;
    }
    if (root->value.child == CF_NULL_PTR) {
        root->value.child = node;
    } else {
        tmp = root->value.child;
        while (tmp->next) {
            tmp = tmp->next;
        }
        tmp->next = node;
    }
    return node;
}

cfx_json_t* cfx_json_object_add(cfx_json_t* root, const cf_char_t* name, cfx_json_t* node) {
    cfx_json_t* tmp = CF_NULL_PTR;
    if (root->type != CFX_JSON_VALUE_TYPE_OBJECT) {
        return CF_NULL_PTR;
    }
    node->name = cf_malloc_z(cf_strlen(name) + 1);
    cf_strcpy_s(node->name, cf_strlen(name) + 1, name);

    if (root->value.child == CF_NULL_PTR) {
        root->value.child = node;
    } else {
        tmp = root->value.child;
        while (tmp->next) {
            tmp = tmp->next;
        }
        tmp->next = node;
    }
    return node;

}

cfx_json_t* cfx_json_get(cfx_json_t* self, const cf_char_t* name) {
    cfx_json_t* tmp = CF_NULL_PTR;
    if (self->type != CFX_JSON_VALUE_TYPE_OBJECT) {
        return CF_NULL_PTR;
    }
    if (!self->value.child) {
        return CF_NULL_PTR;
    }
    tmp = self->value.child;

    while (tmp) {
        if (cf_strcmp(tmp->name, name) == 0) {
            return tmp;
        }
    }
    return CF_NULL_PTR;
}

cfx_json_t* cfx_json_get_cascade(cfx_json_t* self, const cf_char_t* name) {
    return CF_NULL_PTR;
}

