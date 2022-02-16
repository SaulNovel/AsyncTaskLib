project(tasklib)

set(LIBRARY tasklib)

set(HEADERS
    Task.h
    Scheduler.h
    StopException.h
    # Example tasks
    TestTask.h
    Counter.h
    Fibonacci.h
)

set(SOURCES
    Task.cpp
    Scheduler.cpp
)

find_package(Threads REQUIRED)

add_library(${LIBRARY} STATIC
    ${SOURCES}
    ${HEADERS}
)

target_link_libraries(${LIBRARY} Threads::Threads)