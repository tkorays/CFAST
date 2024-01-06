#include "cfx/json.h"

#include "cf/memory.h"
#include "cf/str.h"


cfx_json_t* cfx_json_new() {
    cfx_json_t* json = cf_malloc_z(sizeof(cfx_json_t));
    json->type = CFX_JSON_VALUE_TYPE_OBJECT;
    return json;
}

void cfx_json_delete(cfx_json_t* self) {
    cfx_json_t* first = CF_NULL_PTR;
    if (self->type == CFX_JSON_VALUE_TYPE_OBJECT ||
        self->type == CFX_JSON_VALUE_TYPE_ARRAY) {
        first = self->child;
        while (first) {
            self->child = first->next;
            cfx_json_delete(first);
            first = self->child; 
        }
    } else if (self->type == CFX_JSON_VALUE_TYPE_STRING) {
        if (self->string) {
            cf_free(self->string);
        }
    } 

    if (self->name) {
        cf_free(self->name);
    }
    cf_free(self);
}

cf_bool_t cfx_json_parse(cfx_json_t* self, const cf_char_t* file);

cf_bool_t cfx_json_save(cfx_json_t* self, const cf_char_t* file);

cf_bool_t cfx_json_parse_str(cfx_json_t* self, const cf_char_t* lines);

cfx_json_t* cfx_json_new_int(int value) {
    cfx_json_t* json = cf_malloc_z(sizeof(cfx_json_t));
    json->type = CFX_JSON_VALUE_TYPE_NUMBER;
    json->number = value;
    return json;
}

cfx_json_t* cfx_json_new_double(double value) {
    cfx_json_t* json = cf_malloc_z(sizeof(cfx_json_t));
    json->type = CFX_JSON_VALUE_TYPE_NUMBER;
    json->number = value;
    return json;
}

cfx_json_t* cfx_json_new_string(const cf_char_t* value) {
    cfx_json_t* json = cf_malloc_z(sizeof(cfx_json_t));
    json->type = CFX_JSON_VALUE_TYPE_STRING;
    json->string = cf_malloc(cf_strlen(value) + 1);
    cf_strcpy_s(json->string, cf_strlen(value) + 1, value);
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
    json->boolean = value;
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
    if (root->type != CFX_JSON_VALUE_TYPE_ARRAY) {
        return CF_NULL_PTR;
    }
    if (root->child == CF_NULL_PTR) {
        root->child = node;
        node->next = CF_NULL_PTR;
        node->prev = node;
    } else {
        // root->child : the first node
        // root->child->prev : the last node
        // root->child->prev->next = node : add node to the last node
        root->child->prev->next = node;
        node->next = CF_NULL_PTR;
        node->prev = root->child->prev;
        root->prev = node;
    }
    return node;
}

cfx_json_t* cfx_json_object_add(cfx_json_t* root, const cf_char_t* name, cfx_json_t* node) {
    if (root->type != CFX_JSON_VALUE_TYPE_OBJECT) {
        return CF_NULL_PTR;
    }
    node->name = cf_malloc_z(cf_strlen(name) + 1);
    cf_strcpy_s(node->name, cf_strlen(name) + 1, name);

    if (root->child == CF_NULL_PTR) {
        root->child = node;
        node->next = CF_NULL_PTR;
        node->prev = node;
    } else {
        // root->child : the first node
        // root->child->prev : the last node
        // root->child->prev->next = node : add node to the last node
        root->child->prev->next = node;
        node->next = CF_NULL_PTR;
        node->prev = root->child->prev;
        root->prev = node;
    }
    return node;

}

cfx_json_t* cfx_json_get(cfx_json_t* self, const cf_char_t* name) {
    return CF_NULL_PTR;
}

