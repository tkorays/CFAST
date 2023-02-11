#include "cf/task_queue.h"
#include "cf/queue.h"
#include "cf/thread.h"
#include "cf/memory.h"
#include "cf/mutex.h"
#include "cf/event.h"

#include <stdio.h>

struct cf_task_queue {
    cf_lite_queue_t tasks;
    cf_lite_queue_t pending_tasks;
    cf_thread_t     thread;
    cf_mutex_t      mutex;
    cf_event_t*     notify;
};

CF_THREAD_DEF_PROC(task_queue_proc, args) {
    while (CF_TRUE) {
        break;
    }
    return CF_THREAD_RET(0);
}

cf_task_queue_t* cf_task_queue_new(const char* name, int priority) {
    cf_thread_t thread;
    cf_thread_attr_t attr;
    cf_task_queue_t* task_queue = CF_NULL_PTR;

    task_queue = cf_malloc_z(sizeof(cf_task_queue_t));
    if (!task_queue) {
        return CF_NULL_PTR;
    }

    if (!cf_lite_queue_init(&task_queue->tasks, 100, 10)) {
        goto err_proc;
    }
    if (!cf_lite_queue_init(&task_queue->pending_tasks, 100, 10)) {
        goto err_proc;
    }

    if (CF_OK != cf_mutex_init(&task_queue->mutex, CF_NULL_PTR)) {
        goto err_proc;
    }

    task_queue->notify = cf_event_new(CF_FALSE, CF_FALSE);
    if (!task_queue->notify) {
        goto err_proc;
    }

    cf_thread_attr_init(&attr);
    if (CF_OK != cf_thread_create(&thread, &attr, task_queue_proc, 0)) {
        goto err_proc;
    }
    cf_thread_setname(thread, name);
    cf_thread_set_priority(thread, priority);
    cf_thread_attr_destroy(&attr);

    return task_queue;
err_proc:
    if (task_queue) {
        cf_lite_queue_deinit(&task_queue->tasks);
        cf_lite_queue_deinit(&task_queue->pending_tasks);
        cf_mutex_destroy(&task_queue->mutex);
        cf_thread_attr_destroy(&attr);
        if (task_queue->notify) {
            cf_event_delete(task_queue->notify);
        }
        cf_free(task_queue);
    } 
    return CF_NULL_PTR;
}

void cf_task_queue_delete(cf_task_queue_t* self) {

}

void cf_task_queue_post(
    cf_task_queue_t* self,
    cf_task_queue_task_t task) {
    cf_mutex_lock(&self->mutex);
    {
        cf_lite_queue_push_back(&self->pending_tasks, CF_NULL_PTR);
    }
    cf_mutex_unlock(&self->mutex);
}

void cf_task_queue_post_delayed(
    cf_task_queue_t* self,
    cf_task_queue_task_t task,
    int ms) {
    cf_mutex_lock(&self->mutex);
    {
        cf_lite_queue_push_back(&self->pending_tasks, CF_NULL_PTR);
    }
    cf_mutex_unlock(&self->mutex);
}
