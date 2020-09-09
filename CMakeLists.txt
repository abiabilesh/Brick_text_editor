cmake_minimum_required(VERSION 3.1.0)

project(BrickTextEditor)

find_package(Threads REQUIRED)

set(THREADS_PREFER_PTHREAD_FLAG ON)

set(CORE_MODULE_DIR ${CMAKE_SOURCE_DIR}/core_module/)
set(HARDWARE_MODULE_DIR ${CMAKE_SOURCE_DIR}/hardware_module/)
set(UTILITIES_DIR ${CMAKE_SOURCE_DIR}/utilities/)
set(WINDOW_MODULE_DIR ${CMAKE_SOURCE_DIR}/utilities/)

file(GLOB_RECURSE CORE_MODULE_SRC ${CORE_MODULE_DIR} *.c)
file(GLOB_RECURSE HARDWARE_MODULE_SRC ${HARDWARE_MODULE_DIR} *.c)
file(GLOB_RECURSE UTILITIES_SRC ${UTILITIES_DIR} *.c)
file(GLOB_RECURSE WINDOW_MODULE_SRC ${WINDOW_MODULE_DIR} *.c)

MACRO(HEADER_DIRECTORIES return_list)
    FILE(GLOB_RECURSE new_list *.h)
    SET(dir_list "")
    FOREACH(file_path ${new_list})
        GET_FILENAME_COMPONENT(dir_path ${file_path} PATH)
        SET(dir_list ${dir_list} ${dir_path})
    ENDFOREACH()
    LIST(REMOVE_DUPLICATES dir_list)
    SET(${return_list} ${dir_list})
ENDMACRO()

HEADER_DIRECTORIES(header_dir_list)

list(LENGTH header_dir_list header_dir_list_count)
message(STATUS "[INFO] Found ${header_dir_list_count} header directories.")

set(SOURCES ${CORE_MODULE_SRC}
            ${HARDWARE_MODULE_SRC}
            ${UTILITIES_SRC}
            ${WINDOW_MODULE_SRC}
   )

set(EXECUTABLE brick)
   
include_directories(${header_dir_list})

add_library(${EXECUTABLE} ${SOURCES})

target_link_libraries(${EXECUTABLE} PRIVATE Threads::Threads)

install(TARGETS ${EXECUTABLE} DESTINATION /usr/bin)
