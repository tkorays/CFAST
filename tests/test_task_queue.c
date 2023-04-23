#include "cf/task_queue.h"
#include "cf/time.h"
#include <stdio.h>

void task1(void* data) {
    printf("this is the task 1\n");
}

void delay_task1(void* data) {
    printf("this is delayed task 1\n");
}


int main(int argc, char const *argv[])
{
    cf_task_queue_t* task_queue = cf_task_queue_new("wtf", 0);
    
    cf_task_queue_task_t task;
    cf_task_queue_task_t delay_task;
    task.data = CF_NULL_PTR;
    task.run = task1;

    delay_task.data = CF_NULL_PTR;
    delay_task.run = delay_task1;

    cf_task_queue_post_delayed(task_queue, delay_task, 5);
    cf_task_queue_post(task_queue, task);
    cf_task_queue_post(task_queue, task);

    /** wait for the delayed task done */
    cf_time_sleep(20);
    cf_task_queue_delete(task_queue);
    return 0;
}
