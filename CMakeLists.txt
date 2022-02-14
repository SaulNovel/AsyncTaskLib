cmake_minimum_required(VERSION 3.8)
project(AsyncTaskLib)

SET(GCC_COVERAGE_COMPILE_FLAGS "-fpermissive -Wno-deprecated-declarations -fexceptions -g  -Wall -Wno-long-long -Wconversion -Wwrite-strings -Wsign-compare -Dgtest_disable_pthreads=OFF")

set(CMAKE_CXX_STANDARD 14)
set(SOURCES ${SOURCES})
set(THREADS_PREFER_PTHREAD_FLAG ON)

add_definitions(${GCC_COVERAGE_COMPILE_FLAGS})

include_directories(src)
include_directories(test)

add_subdirectory(src)
add_subdirectory(test)