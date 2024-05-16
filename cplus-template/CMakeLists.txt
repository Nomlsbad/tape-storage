#App
set(APP_NAME cplus-template)

set(APP_PATH "${CMAKE_CURRENT_LIST_DIR}/app")
set(APP_SRC_PATH "${APP_PATH}/src")
set(APP_INCLUDE_PATH "${APP_PATH}/include")
set(APP_MAIN "${APP_SRC_PATH}/main.cpp")

file(GLOB_RECURSE APP_SOURCES CONFIGURE_DEPENDS "${APP_SRC_PATH}/*.cpp")
list(FILTER APP_SOURCES EXCLUDE REGEX "${APP_SRC_PATH}/*/main.cpp")
message(STATUS ${APP_SOURCES})

add_subdirectory(app)

#Tests
set(TESTS_NAME ${APP_NAME}-unit-tests)

set(TESTS_PATH "${CMAKE_CURRENT_LIST_DIR}/tests")
set(TESTS_SRC_PATH ${TESTS_PATH})

file(GLOB_RECURSE TESTS_SOURCES "${TESTS_SRC_PATH}/*.cpp")
message(STATUS ${TESTS_SOURCES})

add_subdirectory(tests)