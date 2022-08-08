cmake_minimum_required(VERSION 3.2)

project(cfast C CXX)

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
    src/core/vector.c
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


set(CMAKE_CXX_STANDARD 11)
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()
file(
    GLOB_RECURSE CFAST_TEST_SRC
    ${CMAKE_SOURCE_DIR}/tests/*_test.cpp
)
add_executable(
    cfast_unittest
    ${CFAST_TEST_SRC}
)
target_link_libraries(cfast_unittest gtest_main cfast)
include(GoogleTest)
gtest_discover_tests(cfast_unittest)