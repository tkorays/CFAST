#include "cf/task_queue.h"
#include "cf/queue.h"
#include "cf/array.h"
#include "cf/thread.h"
#include "cf/memory.h"
#include "cf/mutex.h"
#include "cf/event.h"
#include "cf/err.h"
#include "cf/time.h"
#include "cf/clock.h"

#include <stdio.h>

#define CF_TASK_QUEUE_INIT_SIZE 16

struct cf_task_queue {
    cf_lite_queue_t tasks;
    //cf_lite_queue_t delayed_tasks;
    cf_array_t      delayed_tasks;
    cf_thread_t     thread;
    cf_mutex_t      mutex;
    cf_event_t*     notify;
    cf_bool_t       exit;
};

CF_THREAD_DEF_PROC(task_queue_proc, args) {
    cf_task_queue_t* task_queue = CF_TYPE_CAST(cf_task_queue_t*, args);
    cf_task_queue_task_t* task = CF_NULL_PTR;
    int wait_ms = CF_MAX_INT32;
    cf_int64_t now_ms;
    cf_size_t idx, cnt;

    printf("task_queue start\n");

    while (CF_TRUE) {
        if (task_queue->exit) {
            break;
        }

        now_ms = cf_clock_current_ms(cf_clock_global());

        /* check the delayed task */
        idx = 0;
        while (CF_TRUE) {
            cf_mutex_lock(&task_queue->mutex);
            cnt = cf_array_size(&task_queue->delayed_tasks);
            if (idx >= cnt) {
                cf_mutex_unlock(&task_queue->mutex);
                break;
            }

            task = CF_TYPE_CAST(cf_task_queue_task_t*, cf_array_get(&task_queue->delayed_tasks, idx));
            
            if (now_ms >= task->due_time) {
                task->due_time = 0;
                cf_lite_queue_push_back(&task_queue->tasks, task);

                cf_array_erase(&task_queue->delayed_tasks, idx);
            } else {
                wait_ms = CF_MIN2(task->due_time - now_ms, wait_ms);
                idx++;
            }
            cf_mutex_unlock(&task_queue->mutex);
        }

        /* process the realtime task */
        while (CF_TRUE) {
            cf_mutex_lock(&task_queue->mutex);
            if (cf_lite_queue_empty(&task_queue->tasks)) {
                cf_mutex_unlock(&task_queue->mutex);
                break;
            }
            task = CF_TYPE_CAST(cf_task_queue_task_t*, cf_lite_queue_pop_front(&task_queue->tasks));
            cf_mutex_unlock(&task_queue->mutex);

            if (task) {
                task->run(task->data);
                if (task->data) {
                    cf_free(task->data);
                }
            }
        }

        /* process the delayed task */
        wait_ms = wait_ms == CF_MAX_INT32 ? -1 : wait_ms;
        cf_event_wait(task_queue->notify, wait_ms);
    }
    printf("task_queue end\n");
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
    task_queue->exit = CF_FALSE;

    if (!cf_lite_queue_init(&task_queue->tasks, sizeof(cf_task_queue_task_t), CF_TASK_QUEUE_INIT_SIZE)) {
        goto err_proc;
    }
    if (!cf_array_init(&task_queue->delayed_tasks, sizeof(cf_task_queue_task_t), CF_TASK_QUEUE_INIT_SIZE)) {
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
    if (CF_OK != cf_thread_create(&thread, &attr, task_queue_proc, task_queue)) {
        goto err_proc;
    }
    cf_thread_setname(thread, name);
    cf_thread_set_priority(thread, priority);
    cf_thread_attr_destroy(&attr);

    task_queue->thread = thread;

    return task_queue;
err_proc:
    if (task_queue) {
        cf_lite_queue_deinit(&task_queue->tasks);
        cf_array_deinit(&task_queue->delayed_tasks);
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
    self->exit = CF_TRUE;
    cf_event_set(self->notify);

    cf_thread_join(self->thread, 0);
    cf_mutex_lock(&self->mutex);
    {
        cf_lite_queue_deinit(&self->tasks);
        cf_array_deinit(&self->delayed_tasks);
        cf_event_delete(self->notify);
    }
    cf_mutex_unlock(&self->mutex);

    cf_mutex_destroy(&self->mutex);
    cf_free(self);
}

void cf_task_queue_post(
    cf_task_queue_t* self,
    cf_task_queue_task_t task) {
    cf_mutex_lock(&self->mutex);
    {
        task.due_time = 0;
        cf_lite_queue_push_back(&self->tasks, &task);
    }
    cf_mutex_unlock(&self->mutex);
    if (self->exit) return;
    cf_event_set(self->notify);
}

void cf_task_queue_post_delayed(
    cf_task_queue_t* self,
    cf_task_queue_task_t task,
    int ms) {
    cf_mutex_lock(&self->mutex);
    {
        task.due_time = cf_clock_current_ms(cf_clock_global()) + ms;
        cf_array_insert(&self->delayed_tasks, CF_ARRAY_END_INDEX, &task, sizeof(task));
    }
    cf_mutex_unlock(&self->mutex);
    if (self->exit) return;
    cf_event_set(self->notify);
}
