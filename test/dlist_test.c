#include <stdio.h>
#include <stdlib.h>
#include <cfast/cf_list_if.h>


int main() {
    cf_list_t* li = CF_NULL_PTR;
    cf_int32_t arr[] = {1,2,3,4,5};
    cf_int32_t *data = CF_NULL_PTR;
    cf_list_iter_t it = CF_NULL_PTR;

    li = cf_list_create(free);
    cf_list_insert(li, arr, CF_LIST_POS_TAIL);
    cf_list_insert(li, arr+1, CF_LIST_POS_TAIL);
    cf_list_insert(li, arr+2, CF_LIST_POS_TAIL);
    cf_list_insert(li, arr+3, CF_LIST_POS_HEAD);
    cf_list_insert(li, arr+4, CF_LIST_POS_HEAD);
    cf_list_delete(li, CF_LIST_POS_TAIL, CF_FALSE);
    cf_list_delete(li, CF_LIST_POS_HEAD, CF_FALSE);
    cf_list_delete(li, 1, CF_FALSE);
    printf("list size: %u\n", cf_list_size(li));
    cf_list_iter_init(li, &it);
    while(it) {
        cf_list_iter_data(it, (cf_void_t**)&data);
        printf("data: %d\n", *data);
        it = cf_list_iter_next(it);
    }
    cf_list_destroy(li, CF_FALSE);

    return 0;
}