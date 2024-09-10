#include "cf/map.h"
#include "cf/assert.h"
#include <stdio.h>


int main(int argc, char *argv[]) {
    cf_map_t* m = cf_map_new(); 

    cf_map_delete(m);
    return 0;
}
