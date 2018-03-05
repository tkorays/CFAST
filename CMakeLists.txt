cmake_minimum_required(VERSION 3.2)

project(cfast C)

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
    src/common/poll.c
    )

add_library(cfast STATIC ${SRC_LIST})
if(WIN32)
    #include_directories(3rdparty/pthread2.9.1/include)
    #link_directories(3rdparty/pthread2.9.1/lib/x86)
    add_definitions("-DHAVE_STRUCT_TIMESPEC")
    #target_link_libraries(cfast)
else()
target_link_libraries(cfast pthread)
endif()

install(TARGETS cfast
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION libstatic)
install(DIRECTORY include DESTINATION .)

if(WIN32)
install(DIRECTORY 3rdparty/pthread2.9.1/dll/x86 DESTINATION bin)
endif()
