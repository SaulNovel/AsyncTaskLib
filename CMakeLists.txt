cmake_minimum_required(VERSION 3.8)

project(AsyncTaskLib)

set(CMAKE_CXX_STANDARD 14)
set(THREADS_PREFER_PTHREAD_FLAG ON)

SET(GCC_COVERAGE_COMPILE_FLAGS "-fpermissive -Wno-deprecated-declarations -fexceptions -g  -Wall -Wno-long-long -Wconversion -Wwrite-strings -Wsign-compare -Dgtest_disable_pthreads=OFF")
add_definitions(${GCC_COVERAGE_COMPILE_FLAGS})

enable_testing()

include_directories(tasklib)

add_subdirectory(tasklib)
add_subdirectory(test)
add_subdirectory(cli)