#ifndef __CFAST_H__
#define __CFAST_H__

#define CF_VERSION_MAJOR 0
#define CF_VERSION_MINOR 1
#define CF_VERSION ((CF_VERSION_MAJOR << 16) | CF_VERSION_MINOR)

#ifndef CF_API
#  ifdef _WIN32
#     if defined(CF_BUILD_SHARED) /* build dll */
#         define CF_API __declspec(dllexport)
#     elif !defined(CF_BUILD_STATIC) /* use dll */
#         define CF_API __declspec(dllimport)
#     else /* static library */
#         define CF_API
#     endif
#  else
#     if __GNUC__ >= 4
#         define CF_API __attribute__((visibility("default")))
#     else
#         define CF_API
#     endif
#  endif
#endif

#endif /* __CFAST_H__ */
