#include <stdlib.h>
#include <iostream>
using namespace std;


class LocalPool {
public:
    LocalPool() {
        first = new MemNode(1);
        current = first;
    }
    void Destroy() {
        MemNode *next = first, *tmp;
        while(next) {
            cout<<"delete node"<<endl;
            tmp = next->next;
            delete next;
            next = tmp;
        }
    }
    ~LocalPool() {
        Destroy();
    }

    void* alloc(size_t size) {
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
};

void Abc() {
    LocalPool pool;
    void* p1 = pool.alloc(10);
    void* p2 = pool.alloc(5);
    void* p3 = pool.alloc(100);
}

int main(int argc, char** argv) {
    Abc();
    return 0;
}
