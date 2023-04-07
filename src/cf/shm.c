#include "cf/types.h"


#ifdef CF_OS_WIN
#include "shm_win.c"
#else
#include "shm_linux.c"
#endif