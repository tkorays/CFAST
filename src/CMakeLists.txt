
set(SRC_LIST cf/list.c
    cf/memory.c
    cf/mpool.c
    cf/cli.c
    cf/opt.c 
    cf/str.c
    cf/logging.c
    cf/thread.c
    cf/err.c
    cf/event.c
    cf/file.c
    cf/queue.c
    cf/config.c
    cf/sock.c
    cf/mutex.c
    cf/dso.c
    cf/dbg.c
    cf/select.c
    cf/path.c
    cf/env.c
    cf/time.c
    cf/system.c
    cf/string.c
    cf/vector.c
    cf/clock.c
    cf/array.c
    cf/tls.c
    cf/bytes.c
    cf/task_queue.c
    cf/version.c
    cf/numerics.c
    cf/hashtbl.c
    cf/value.c
    cf/fifobuf.c
    cf/rwlock.c
    cfx/cli.c
    cfx/fec.c
    cfx/ascii_art.c
    cfx/ini.c
    cfx/json.c
    )

if(WIN32)
    set(SRC_LIST ${SRC_LIST}
        cf/shm_win.c)
else()
    set(SRC_LIST ${SRC_LIST}
        cf/poll.c
        cf/shm_linux.c)
endif()

add_library(cfast STATIC ${SRC_LIST})

if ("${CMAKE_C_COMPILER_ID}" STREQUAL "MSVC")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /we4013 /we4431 /we4133 /we4716 /we6244 /we6246 /we4457 /we4456 /we4172 /we4700 /we4477 /we4018 /we4047")
elseif ("${CMAKE_C_COMPILER_ID}" STREQUAL "GNU")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Werror=implicit-function-declaration -Werror=implicit-int -Werror=incompatible-pointer-types -Werror=return-type -Werror=shadow -Werror=return-local-addr -Werror=uninitialized -Werror=format -Werror=sign-compare -Werror=int-conversion")
elseif ("${CMAKE_C_COMPILER_ID}" MATCHES "Clang")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Werror=implicit-function-declaration -Werror=implicit-int -Werror=incompatible-pointer-types -Werror=return-type -Werror=shadow -Werror=return-stack-address -Werror=uninitialized -Werror=format -Werror=sign-compare -Werror=int-conversion")
endif()

target_include_directories(cfast PUBLIC ../include)
if(WIN32)
    target_compile_definitions(cfast PUBLIC HAVE_STRUCT_TIMESPEC)
else()
    target_link_libraries(cfast pthread)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(cfast PUBLIC CF_MEMORY_DBG)
endif()

include(TestBigEndian)
TEST_BIG_ENDIAN(IS_BIG_ENDIAN)
if(IS_BIG_ENDIAN)
    target_compile_definitions(cfast PUBLIC CF_IS_BIG_ENDIAN)
else()
    target_compile_definitions(cfast PUBLIC CF_IS_LITTLE_ENDIAN)
endif()

if(NOT CMAKE_INSTALL_PREFIX)
    set(CMAKE_INSTALL_PREFIX /usr/local)
endif()

install(TARGETS cfast
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION libstatic)
install(DIRECTORY include DESTINATION .)

if(WIN32)
    install(DIRECTORY DESTINATION bin)
endif()
