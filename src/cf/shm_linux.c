#include "cf/shm.h"
#include "cf/types.h"
#include "cf/err.h"
#include "cf/memory.h"
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

struct cf_shm {
    void* buf;
    size_t size;
    int fd;
};

CF_DECLARE(cf_errno_t) cf_shm_create(cf_shm_t** shm, cf_size_t size, const cf_char_t* filename) {
    int fd = shm_open(filename, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        return CF_ENOK;
    }
    if (ftruncate(fd, size) == -1) {
        close(fd);
        return CF_ENOK;
    }
    void* buf = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (buf == MAP_FAILED) {
        close(fd);
        return CF_ENOK;
    }
    *shm = (cf_shm_t*)cf_malloc(sizeof(cf_shm_t));
    (*shm)->buf = buf;
    (*shm)->size = size;
    (*shm)->fd = fd;
    return CF_EOK;
}

CF_DECLARE(cf_errno_t) cf_shm_destroy(cf_shm_t* shm) {
    if (munmap(shm->buf, shm->size) == -1) {
        return CF_ENOK;
    }
    if (close(shm->fd) == -1) {
        return CF_ENOK;
    }
    cf_free(shm);
    return CF_EOK;
}

CF_DECLARE(cf_void_t*) cf_shm_get_buf(cf_shm_t* shm) {
    return shm->buf;
}

CF_DECLARE(cf_size_t)  cf_shm_get_size(cf_shm_t* shm) {
    return shm->size;
}

