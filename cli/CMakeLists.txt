project(cli)

set(BINARY program_cli)

Set(SOURCES
    main.cpp
)

option(USE_STATIC_BOOST "Build with static BOOST libraries instead of dynamic" YES)
set(Boost_USE_STATIC_LIBS ${USE_STATIC_BOOST})
unset(Boost_INCLUDE_DIR CACHE)
unset(Boost_LIBRARY_DIRS CACHE)
find_package(Boost COMPONENTS program_options REQUIRED)
find_package(Boost 1.65.1 COMPONENTS program_options) 
if(USE_STATIC_BOOST)
   message(STATUS "Linking against boost static libraries")
else()
   message(STATUS "Linking against boost dynamic libraries")
endif()

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