set(BINARY program_cli)

file(GLOB_RECURSE SOURCES LIST_DIRECTORIES true *.h *.cpp)

find_package(Boost COMPONENTS program_options REQUIRED)
find_package(Boost 1.65.1 COMPONENTS program_options) 
include_directories(${Boost_INCLUDE_DIRS}) 

add_executable(
    ${BINARY} 
    ${SOURCES}
)

target_link_libraries(
    ${BINARY}
    ${Boost_LIBRARIES}
    tasklib
)