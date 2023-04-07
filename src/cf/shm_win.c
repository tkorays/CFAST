#include "cf/shm.h"
#include "cf/types.h"
#include "cf/memory.h"

struct cf_shm {
    HANDLE handle;
    LPCTSTR buf;
    cf_size_t size;
};


CF_DECLARE(cf_errno_t) cf_shm_create(cf_shm_t** shm, cf_size_t size, const cf_char_t* filename) {
    HANDLE hMapFile;
    LPCTSTR pBuf;

    hMapFile = CreateFileMapping(
        INVALID_HANDLE_VALUE,    // use paging file
        NULL,                    // default security
        PAGE_READWRITE,          // read/write access
        0,                       // maximum object size (high-order DWORD)
        size,                // maximum object size (low-order DWORD)
        filename);                 // name of mapping object

    if (hMapFile == NULL)
    {
        return CF_ENOK;
    }

    pBuf = (LPTSTR)MapViewOfFile(hMapFile,   // handle to map object
        FILE_MAP_ALL_ACCESS, // read/write permission
        0,
        0,
        size);

    if (pBuf == NULL)
    {
        CloseHandle(hMapFile);
        return CF_ENOK;
    }

    *shm = (cf_shm_t*)cf_malloc(sizeof(cf_shm_t));
    (*shm)->handle = hMapFile;
    (*shm)->buf = pBuf;
    (*shm)->size = size;

    return CF_EOK;
}

CF_DECLARE(cf_errno_t) cf_shm_destroy(cf_shm_t* shm) {
    UnmapViewOfFile(shm->buf);
    CloseHandle(shm->handle);
    cf_free(shm);
    return CF_EOK;
}

CF_DECLARE(cf_void_t*) cf_shm_get_buf(cf_shm_t* shm) {
    return shm->buf;
}

CF_DECLARE(cf_size_t)  cf_shm_get_size(cf_shm_t* shm) {
    return shm->size;
}



