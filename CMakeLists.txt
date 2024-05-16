cmake_minimum_required(VERSION 3.26)

project(cplus_template LANGUAGES CXX)

include(cmake/CompileOptions.cmake)
include(cmake/Sanitize.cmake)
include(cmake/Libs.cmake)
include(cmake/Werror.cmake)
include(cmake/Testing.cmake)

add_subdirectory(cplus-template)