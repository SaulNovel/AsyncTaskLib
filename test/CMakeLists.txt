set(BINARY program_test)

file(GLOB_RECURSE TEST_SOURCES LIST_DIRECTORIES true *.h *.cpp)

add_subdirectory(googletest)

add_executable(
    ${BINARY} 
    ${TEST_SOURCES}
)

target_link_libraries(
    ${BINARY} 
    gtest_main
    tasklib
)

include(GoogleTest)
gtest_discover_tests(${BINARY})