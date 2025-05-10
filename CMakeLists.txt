cmake_minimum_required(VERSION 4.0)

project(cdb)

set(SOURCE_EXE src/main.c)

set(SOURCE_LIB src/db.c src/hashtable.c src/server.c)

add_library(cdb_lib STATIC ${SOURCE_LIB})

add_executable(cdb src/main.c)

target_link_libraries(cdb cdb_lib)