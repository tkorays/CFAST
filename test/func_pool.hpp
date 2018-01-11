#pragma once

#include <cstdlib>

namespace jx {

class LocalPool {
public:
    LocalPool() {
        first = new MemNode(1);
        current = first;
    }
    ~LocalPool() {
        MemNode *next = first, *tmp;
        while(next) {
            tmp = next->next;
            delete next;
            next = tmp;
        }
    }

    void* alloc(size_t size) {
        if(!current) return 0;
        current->next = new MemNode(size);
        current = current->next;
        return (void*)current->data;
    }
private:
    struct MemNode {
        char* data;
        struct MemNode *next;
        MemNode(size_t size) {
            data = new char[size];
            next = 0;
        }
    } *first, *current;
    void* operator new(size_t size) {return (void*)0xFFFFFFF;}
};

}

void a() {
    jx::LocalPool pool;
    pool.alloc(5);
}
