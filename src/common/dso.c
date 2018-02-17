#include <cf/types.h>

#ifdef CF_OS_WIN 
#include "dso_win.c"
#else 
#include "dso_linux.c"
#endif