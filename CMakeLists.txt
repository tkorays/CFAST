cmake_minimum_required(VERSION 3.2)

project(cfast C)

option(USE_CFEXP_LIBTELNET_SERVER "use libtelnet server in cfexp"  OFF)

include_directories(./include)

if(NOT CMAKE_INSTALL_PREFIX)
    set(CMAKE_INSTALL_PREFIX /usr/local)
endif()

set(SRC_LIST src/core/list.c
    src/core/memory.c
    src/core/mpool.c
    src/core/cli.c
    src/core/opt.c 
    src/core/str.c
    src/core/log.c
    src/core/thread.c
    src/core/err.c
    src/core/file.c
    src/core/queue.c
    src/core/config.c
    src/core/socket.c
    src/core/mutex.c
    src/core/dso.c
    src/core/dbg.c
    src/core/select.c
    src/core/path.c
    src/core/env.c
    src/core/time.c
    src/core/system.c
    src/core/string.c
    )

if(USE_CFEXP_LIBTELNET_SERVER)
    set(SRC_LIST ${SRC_LIST}   
        src/cfexp/telnet_server.c
        3rdparty/libtelnet/libtelnet.c)
endif()

if(WIN32)

else()
    set(SRC_LIST ${SRC_LIST}
        src/core/poll.c)
endif()

add_library(cfast STATIC ${SRC_LIST})
if(WIN32)
    add_definitions("-DHAVE_STRUCT_TIMESPEC")
else()
target_link_libraries(cfast pthread)
endif()

install(TARGETS cfast
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION libstatic)
install(DIRECTORY include DESTINATION .)

if(WIN32)
install(DIRECTORY DESTINATION bin)
endif()
