cmake_minimum_required(VERSION 3.2)

project(cfast C)

option(USE_CFEXP_LIBTELNET_SERVER "use libtelnet server in cfexp"  OFF)

include_directories(./include)

if(NOT CMAKE_INSTALL_PREFIX)
    set(CMAKE_INSTALL_PREFIX /usr/local)
endif()

set(SRC_LIST src/common/list.c
    src/common/memory.c
    src/common/mpool.c
    src/common/cli.c
    src/common/opt.c 
    src/common/str.c
    src/common/log.c
    src/common/thread.c
    src/common/err.c
    src/common/file.c
    src/common/queue.c
    src/common/config.c
    src/common/socket.c
    src/common/mutex.c
    src/common/dso.c
    src/common/dbg.c
    src/common/select.c
    src/common/path.c
    src/common/env.c
    )

if(USE_CFEXP_LIBTELNET_SERVER)
    set(SRC_LIST ${SRC_LIST}   
        src/cfexp/telnet_server.c
        3rdparty/libtelnet/libtelnet.c)
endif()

if(WIN32)

else()
    set(SRC_LIST ${SRC_LIST}
        src/common/poll.c)
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
