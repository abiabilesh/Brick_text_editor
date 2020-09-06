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

set(INCLUDE ${HARDWARE_MODULE_DIR}/termios_manager)

set(SOURCES ${CORE_MODULE_SRC}
            ${HARDWARE_MODULE_SRC}
            ${UTILITIES_SRC}
            ${WINDOW_MODULE_SRC}
   )

set(EXECUTABLE brick)
   
include_directories(${INCLUDE})

add_library(${EXECUTABLE} ${SOURCES})

target_link_libraries(${EXECUTABLE} PRIVATE Threads::Threads)

install(TARGETS ${EXECUTABLE} DESTINATION /usr/bin)
