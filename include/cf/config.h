#ifndef __CF_CONFIG_H__
#define __CF_CONFIG_H__

#ifndef CF_API
#  ifdef _WIN32
#       define CF_OS_WIN 1
#       if defined(CF_BUILD_SHARED) /* build dll */
#           define CF_API __declspec(dllexport)
#       elif !defined(CF_BUILD_STATIC) /* use dll */
#           define CF_API __declspec(dllimport)
#       else /* static library */
#         define CF_API
#       endif
#  else
#       define CF_OS_LINUX 1
#       if __GNUC__ >= 4
#           define CF_API __attribute__((visibility("default")))
#       else
#           define CF_API
#       endif
#  endif
#endif

#ifndef CF_DEBUG
#  ifndef NDEBUG
#    define CF_DEBUG 1
#  else
#    define CF_DEBUG 0
#  endif
#endif

#if (defined CF_DEBUG) && (CF_DEBUG != 0)
#  define CF_HAS_ASSERT 1
#endif

#define CF_VERSION_NUM_MAJOR 1
#define CF_VERSION_NUM_MINOR 0
#define CF_VERSION_NUM ((CF_VERSION_MAJOR << 16) | CF_VERSION_MINOR)


const char* cf_get_version();

#endif /* __CF_CONFIG_H__ */
