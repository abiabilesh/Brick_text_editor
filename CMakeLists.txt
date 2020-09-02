cmake_minimum_required(VERSION 3.1.0)

project(BrickTextEditor)

find_package(Threads REQUIRED)

set(THREADS_PREFER_PTHREAD_FLAG ON)

file(GLOB CORE ${CMAKE_SOURCE_DIR}/core/*.c)
file(GLOB FILE ${CMAKE_SOURCE_DIR}/file/*.c)
file(GLOB SYSTEM ${CMAKE_SOURCE_DIR}/system/*.c)
file(GLOB SRC ${CMAKE_SOURCE_DIR}/*.c)

set(INCLUDE ${CMAKE_SOURCE_DIR}/include)

set(SOURCES ${CORE}
            ${FILE}
            ${SYSTEM}
            ${SRC}
   )

set(EXECUTABLE brick)
   
include_directories(${INCLUDE})

add_executable(${EXECUTABLE} ${SOURCES})

target_link_libraries(${EXECUTABLE} PRIVATE Threads::Threads)

install(TARGETS ${EXECUTABLE} DESTINATION /usr/bin)
