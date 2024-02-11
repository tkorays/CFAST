#include "cfx/json.h"

#include "cf/memory.h"
#include "cf/str.h"
#include "cf/file.h"


typedef struct {
    char a;
} json_parse_context;


cfx_json_t* cfx_json_new() {
    cfx_json_t* json = cf_malloc_z(sizeof(cfx_json_t));
    json->type = CFX_JSON_VALUE_TYPE_OBJECT;
    return json;
}

void cfx_json_delete(cfx_json_t* self) {
    cfx_json_t* first = CF_NULL_PTR;
    if (self->type == CFX_JSON_VALUE_TYPE_OBJECT ||
        self->type == CFX_JSON_VALUE_TYPE_ARRAY) {
        first = self->value.child;
        while (first) {
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

cf_bool_t cfx_json_load(cfx_json_t* self, const cf_char_t* file) {
    return CF_TRUE;
}

cf_bool_t cfx_json_save(cfx_json_t* self, const cf_char_t* file) {
    return CF_TRUE;
}

cf_bool_t cfx_json_parse(cfx_json_t* self, const cf_char_t* lines) {
    return CF_TRUE;
}

cf_size_t cfx_json_dump(cfx_json_t* self, cf_char_t* buf, cf_size_t size) {
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
            writed_size += cf_snprintf(buf + writed_size, size - writed_size, "%s:", child->name);
            writed_size += cfx_json_dump(child, buf + writed_size, size - writed_size);
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
            writed_size += cfx_json_dump(child, buf + writed_size, size - writed_size);
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

