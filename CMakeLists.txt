cmake_minimum_required(VERSION 3.12)

project(cfast
        LANGUAGES C
        VERSION 1.0
        DESCRIPTION "CFAST is a c library.")

option(CF_ENABLE_TEST "Enable Unittest" ON)
option(CF_ENABLE_COV "Enable Coverage" OFF)
option(CF_ENABLE_SANTIZE_ADDR "Enable SantizeAddress" OFF)

if (CF_ENABLE_COV)
    add_compile_options(-g -O0 -Wall -fno-inline -fprofile-arcs -ftest-coverage -fprofile-instr-generate -fcoverage-mapping)
    add_link_options(--coverage)
endif()
if (CF_ENABLE_SANTIZE_ADDR)
    add_compile_definitions(-fsanitize=address)
endif()
if (CF_ENABLE_TEST)
    enable_testing()
endif()

if("${CMAKE_BUILD_TYPE}" STREQUAL "")
    SET(CMAKE_BUILD_TYPE Debug)
endif()

add_subdirectory(src)
add_subdirectory(examples)
add_subdirectory(tests)
