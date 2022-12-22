cmake_minimum_required(VERSION 3.24)
project(rexStdlib)

set(CMAKE_CXX_STANDARD 20)

set(REX_IMPORT_AS_LIB "yes")

set(REX_SRC_DIR ${PROJECT_SOURCE_DIR}/reXscript)

include(${REX_SRC_DIR}/rex.cmake)

file(GLOB REX_STDLIB_SRC std/*.cpp)

add_library(stdlib SHARED ${REX_STDLIB_SRC} ${REX_SRC})

add_executable(test main.cpp ${REX_SRC})