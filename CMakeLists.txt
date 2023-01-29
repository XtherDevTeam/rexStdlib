cmake_minimum_required(VERSION 3.10)
project(rexStdlib)

set(CMAKE_CXX_FLAGS_RELEASE "-O3")

set(CMAKE_CXX_STANDARD 20)

set(REX_IMPORT_AS_LIB "yes")

set(REX_SRC_DIR ${PROJECT_SOURCE_DIR}/reXscript)

include(${REX_SRC_DIR}/rex.cmake)

file(GLOB REX_STDLIB_SRC std/*.cpp)

add_library(std SHARED std/sqlite/sqlite3.c std/zip/zip.c ${REX_STDLIB_SRC} ${REX_SRC})

add_executable(test main.cpp ${REX_SRC})

find_package(OpenSSL REQUIRED)
target_link_libraries(std OpenSSL::SSL OpenSSL::Crypto)

if (CMAKE_HOST_SYSTEM_NAME MATCHES "Windows")
    file(GLOB FUCK_OPENSSL "${OPENSSL_INCLUDE_DIR}/../*dll*")
    message(Hotpatch for Windows:\ ${OPENSSL_INCLUDE_DIR})
    file(COPY ${FUCK_OPENSSL} DESTINATION ${CMAKE_CURRENT_BINARY_DIR} FOLLOW_SYMLINK_CHAIN)

    set(CMAKE_CXX_FLAGS "-static -lpthread")
    set(CMAKE_SHARED_LINKER_FLAGS "-static")
    target_link_libraries(std wsock32 ws2_32)
endif ()