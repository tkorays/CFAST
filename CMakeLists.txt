cmake_minimum_required(VERSION 3.2)

project(cfast C CXX)

option(CF_ENABLE_TEST "Enable Unittest" ON)
option(USE_CFEXP_LIBTELNET_SERVER "use libtelnet server in cfexp"  OFF)

if("${CMAKE_BUILD_TYPE}" STREQUAL "")
    SET(CMAKE_BUILD_TYPE Debug)
endif()

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
    src/cf/bytes.c
    src/cfx/cli.c
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

add_executable(mycli examples/mycli.c)
target_link_libraries(mycli cfast)

install(TARGETS cfast
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION libstatic)
install(DIRECTORY include DESTINATION .)

if(WIN32)
install(DIRECTORY DESTINATION bin)
endif()

if(CF_ENABLE_TEST AND CMAKE_BUILD_TYPE STREQUAL "Debug")
    enable_testing()
    set(TESTS
        bytes
        types
        tls
        time
        string
        location
        cli
        queue
    )
    foreach(TEST_NAME ${TESTS})
        add_executable(test_${TEST_NAME} tests/test_${TEST_NAME}.c ${ADDL_SOURCE})
        target_link_libraries(test_${TEST_NAME} cfast)
        message(STATUS ${PROJECT_BINARY_DIR}/test_${TEST_NAME})
        add_test(NAME ${TEST_NAME} COMMAND test_${TEST_NAME})
    endforeach()
endif()

