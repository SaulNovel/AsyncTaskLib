set(LIBRARY tasklib)

file(GLOB_RECURSE SOURCES LIST_DIRECTORIES true *.h *.cpp)

find_package(Threads REQUIRED)
add_library(${LIBRARY} ${SOURCES})
target_link_libraries(${LIBRARY} Threads::Threads)