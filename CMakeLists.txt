cmake_minimum_required(VERSION 3.12)

project(cfast
        LANGUAGES C
        VERSION 1.0
        DESCRIPTION "CFAST is c library.")

option(CF_ENABLE_TEST "Enable Unittest" ON)
option(CF_ENABLE_COV "Enable Coverage" OFF)

if("${CMAKE_BUILD_TYPE}" STREQUAL "")
    SET(CMAKE_BUILD_TYPE Debug)
endif()

if (CF_ENABLE_COV)
    set(CMAKE_C_FLAGS " -fprofile-arcs -ftest-coverage --coverage")
    SET(CMAKE_C_LINK_FLAGS "-fprofile-arcs -fprofile-generate --coverage")
endif()


if(CF_ENABLE_TEST AND CMAKE_BUILD_TYPE STREQUAL "Debug")
    enable_testing()
endif()

add_subdirectory(src)
add_subdirectory(examples)
add_subdirectory(tests)


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