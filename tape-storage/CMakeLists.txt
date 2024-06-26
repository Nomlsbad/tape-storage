#App
set(APP_NAME tape-storage)

set(APP_PATH "${CMAKE_CURRENT_LIST_DIR}")
set(APP_SRC_PATH "${APP_PATH}/src")
set(APP_INCLUDE_PATH "${APP_PATH}/include")
set(APP_MAIN "${APP_SRC_PATH}/main.cpp")

file(GLOB_RECURSE APP_SOURCES CONFIGURE_DEPENDS "${APP_SRC_PATH}/*.cpp")
list(FILTER APP_SOURCES EXCLUDE REGEX "${APP_SRC_PATH}/*/main.cpp")

file(GLOB_RECURSE APP_INCLUDE CONFIGURE_DEPENDS "${APP_INCLUDE_PATH}/*.h")

add_subdirectory(src)

#Tests
set(TESTS_NAME ${APP_NAME}-unit-tests)

set(TESTS_PATH "${CMAKE_CURRENT_LIST_DIR}/tests")
set(TESTS_SRC_PATH ${TESTS_PATH})

file(GLOB_RECURSE TESTS_SOURCES "${TESTS_SRC_PATH}/*.cpp")

add_subdirectory(tests)

#Resources
add_subdirectory(resources)