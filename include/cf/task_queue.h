#ifndef __CF_TASK_QUEUE_H__
#define __CF_TASK_QUEUE_H__

typedef struct cf_task_queue cf_task_queue_t;

typedef void(*cf_queued_task_func_t)(void* ctx);


cf_task_queue_t* cf_task_queue_create(const char* name, int priority);
void cf_task_queue_destroy(cf_task_queue_t* self);

void cf_task_queue_post(void* ctx, cf_queued_task_func_t task);
void cf_task_queue_post_delayed(void* ctx, cf_queued_task_func_t task, int ms);

#endif /* __CF_TASK_QUEUE_H__ */
