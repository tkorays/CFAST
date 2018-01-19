cmake_minimum_required(VERSION 3.2)

project(cfast)

include_directories(./include)

if(NOT CMAKE_INSTALL_PREFIX)
    set(CMAKE_INSTALL_PREFIX /usr/local)
endif()

set(SRC_LIST src/common/list.c
    src/common/memory.c
    src/common/mpool.c
    src/common/cli.c
    src/common/opt.c 
    src/common/string.c
    src/common/str.c
    src/common/log.c
    src/common/thread.c
    src/common/err.c
    src/common/file.c
    src/common/queue.c
    src/common/config.c
    )

add_library(cfast STATIC ${SRC_LIST})
target_link_libraries(cfast pthread)

install(TARGETS cfast
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION libstatic)
install(DIRECTORY include DESTINATION .)
