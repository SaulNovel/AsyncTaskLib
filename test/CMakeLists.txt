set(BINARY program_test)

Set(SOURCES
    mainTests.cpp
)

add_subdirectory(googletest)

add_executable(
    ${BINARY} 
    ${SOURCES}
)

target_link_libraries(${BINARY}
    gtest_main
    tasklib
)

add_test(
    NAME ${BINARY}
    COMMAND ${BINARY}
)