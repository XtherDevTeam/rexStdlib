cmake_minimum_required(VERSION 3.24)
project(rexStdlib)

set(CMAKE_CXX_FLAGS_RELEASE "-O3")

set(CMAKE_CXX_STANDARD 20)

set(REX_IMPORT_AS_LIB "yes")

set(REX_SRC_DIR ${PROJECT_SOURCE_DIR}/reXscript)

include(${REX_SRC_DIR}/rex.cmake)

file(GLOB REX_STDLIB_SRC std/*.cpp)

add_library(std SHARED std/sqlite/sqlite3.c ${REX_STDLIB_SRC} ${REX_SRC})

add_library(std_zipfile SHARED std/zipfile/zipfile.cpp ${REX_SRC})

add_executable(test main.cpp ${REX_SRC})

if (CMAKE_HOST_SYSTEM_NAME MATCHES "Windows")
    set(CMAKE_CXX_FLAGS "-static -lpthread")
    target_link_libraries(std wsock32 ws2_32)
    target_link_libraries(std_zipfile wsock32 ws2_32)
endif ()
