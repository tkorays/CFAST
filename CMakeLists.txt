cmake_minimum_required(VERSION 3.2)

project(cfast)

include_directories(./include)

if(NOT CMAKE_INSTALL_PREFIX)
    set(CMAKE_INSTALL_PREFIX /usr/local)
endif()

set(SRC_LIST src/common/cf_list.c
    src/common/cf_mem.c
    src/common/cf_mpool.c
    src/common/cf_cli.c
    src/common/cf_opt.c 
    src/common/cf_string.c
    src/common/cf_str.c
    src/common/cf_log.c
    src/common/cf_thread.c
    )

add_library(cfast STATIC ${SRC_LIST})
target_link_libraries(cfast pthread)

install(TARGETS cfast
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION libstatic)
install(DIRECTORY include DESTINATION .)
