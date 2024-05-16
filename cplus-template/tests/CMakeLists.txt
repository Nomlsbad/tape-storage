add_executable(${TESTS_NAME} ${TESTS_SOURCES})

target_include_directories(${TESTS_NAME}
        PUBLIC
        ${APP_INCLUDE_PATH}
)

target_link_libraries(${TESTS_NAME}
        PRIVATE
        GTest::gtest
        GTest::gtest_main
        ${APP_NAME}
)

include(GoogleTest)
gtest_discover_tests(${TESTS_NAME})

add_test(NAME ${TESTS_NAME}
        WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
        COMMAND ${TESTS_NAME}
)