cmake_minimum_required(VERSION 3.2)

project(cfast C CXX)

option(USE_CFEXP_LIBTELNET_SERVER "use libtelnet server in cfexp"  OFF)

include_directories(./include)

if(NOT CMAKE_INSTALL_PREFIX)
    set(CMAKE_INSTALL_PREFIX /usr/local)
endif()

set(SRC_LIST src/cf/list.c
    src/cf/memory.c
    src/cf/mpool.c
    src/cf/cli.c
    src/cf/opt.c 
    src/cf/str.c
    src/cf/log.c
    src/cf/thread.c
    src/cf/err.c
    src/cf/file.c
    src/cf/queue.c
    src/cf/config.c
    src/cf/socket.c
    src/cf/mutex.c
    src/cf/dso.c
    src/cf/dbg.c
    src/cf/select.c
    src/cf/path.c
    src/cf/env.c
    src/cf/time.c
    src/cf/system.c
    src/cf/string.c
    src/cf/vector.c
    src/cf/clock.c
    src/cf/array.c
    src/cf/tls.c
    )

if(USE_CFEXP_LIBTELNET_SERVER)
    set(SRC_LIST ${SRC_LIST}   
        src/cfexp/telnet_server.c
        3rdparty/libtelnet/libtelnet.c)
endif()

if(WIN32)

else()
    set(SRC_LIST ${SRC_LIST}
        src/cf/poll.c)
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