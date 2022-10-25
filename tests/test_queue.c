#include "cf/queue.h"
#include "cf/assert.h"

int main(int argc, char const *argv[]) {
    cf_lite_queue_t queue;
    int item;
    cf_lite_queue_init(&queue, sizeof(int), CF_LITE_QUEUE_INIT_CAPACITY);
    cf_assert(cf_lite_queue_count(&queue) == 0);
    item = 1234;
    cf_lite_queue_push_back(&queue, &item);
    cf_assert(*(int*)cf_lite_queue_back(&queue) == item);
    cf_assert(*(int*)cf_lite_queue_front(&queue) == item);

    item = 4567;
    cf_lite_queue_push_back(&queue, &item);
    item = 7890;
    cf_lite_queue_push_back(&queue, &item);
    cf_assert(cf_lite_queue_count(&queue) == 3);

    cf_assert(*(int*)cf_lite_queue_pop_front(&queue) == 1234);
    cf_assert(*(int*)cf_lite_queue_pop_back(&queue) == 7890);
    cf_assert(cf_lite_queue_count(&queue) == 1);

    item = 9876;
    cf_lite_queue_push_front(&queue, &item);
    cf_assert(*(int*)cf_lite_queue_front(&queue) == item);

    cf_lite_queue_clear(&queue);
    cf_assert(cf_lite_queue_count(&queue) == 0);

    for (int i = 0; i < 40; i++) {
        item = i;
        cf_lite_queue_push_back(&queue, &item);
    }
    cf_assert(cf_lite_queue_count(&queue) == 40);
    cf_assert(queue.capacity == CF_LITE_QUEUE_INIT_CAPACITY*2);
    return 0;
}
