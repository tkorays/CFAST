
if(CF_ENABLE_TEST AND CMAKE_BUILD_TYPE STREQUAL "Debug")
    enable_testing()
    set(TESTS
        array
        list
        str
        bytes
        types
        tls
        time
        string
        location
        cli
        queue
        logging
        task_queue
        version
        numerics
        hashtbl
        fec
        sock
        ascii_art
        fifobuf
        ini
        json
    )

    foreach(TEST_NAME ${TESTS})
        add_executable(test_${TEST_NAME} test_${TEST_NAME}.c ${ADDL_SOURCE})
        target_link_libraries(test_${TEST_NAME} cfast)
        message(STATUS ${PROJECT_BINARY_DIR}/tests/test_${TEST_NAME})
        add_test(NAME ${TEST_NAME}
                COMMAND test_${TEST_NAME})
    endforeach()
endif()

