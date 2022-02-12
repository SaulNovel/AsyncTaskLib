set(BINARY program)

file(GLOB_RECURSE SOURCES LIST_DIRECTORIES true *.h *.cpp)

find_package(Threads REQUIRED)
add_executable(${BINARY}_run ${SOURCES})
target_link_libraries(${BINARY}_run Threads::Threads)