add_executable(${TESTS_NAME} ${TESTS_SOURCES})

target_link_libraries(${TESTS_NAME}
        PRIVATE
        GTest::gtest
        GTest::gtest_main
        tape-storage
)

include(GoogleTest)
gtest_discover_tests(${TESTS_NAME})

add_test(NAME ${TESTS_NAME}
        WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
        COMMAND ${TESTS_NAME}
)

target_compile_options(${TESTS_NAME} PUBLIC -DTESTS_DIR="${CMAKE_CURRENT_SOURCE_DIR}")