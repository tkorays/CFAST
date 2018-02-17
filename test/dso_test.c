#include <cf/dso.h>
#include <stdio.h>

typedef cf_void_t (*outerr)(cf_errno_t err, cf_char_t* msg, cf_size_t size);

int main() {
    cf_dso_handle_t* h = CF_NULL_PTR;
    cf_dso_sym_t sym = CF_NULL_PTR;
    cf_errno_t status;
    cf_char_t buff[1024] = {0};
    outerr o;

    cf_dso_load(&h, "../build/abc.so");
    status = cf_dso_sym(h, "cf_err_strerr", &sym);
    printf("status: %u, sym: %p\n", status, sym);
    o = sym;
    o(status, buff, 1024);
    printf("status: %s\n", buff);
    cf_dso_unload(h);
    return 0;
}