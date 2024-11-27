cmake_minimum_required(VERSION 3.25.0)

project(ecs
    VERSION 0.0.1
    DESCRIPTION "R-Type Entity Component System"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Dependencies using CPM 0.40.2
file(
   DOWNLOAD
   https://github.com/cpm-cmake/CPM.cmake/releases/download/v0.40.2/CPM.cmake
   ${CMAKE_CURRENT_BINARY_DIR}/cmake/CPM.cmake
   EXPECTED_HASH SHA256=c8cdc32c03816538ce22781ed72964dc864b2a34a310d3b7104812a5ca2d835d
)
include(${CMAKE_CURRENT_BINARY_DIR}/cmake/CPM.cmake)


file(GLOB_RECURSE SRC "./src/*.cpp")
file(GLOB_RECURSE APP "./include/*.hpp")

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
add_library(${PROJECT_NAME} SHARED ${SRC} ${APP})

target_include_directories(${PROJECT_NAME} PRIVATE include)


CPMAddPackage("gh:SFML/SFML#2.6.1")
target_link_libraries(${PROJECT_NAME} SFML)

target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Werror
    -Wswitch-default -Wcast-align -Wpointer-arith -Wundef -Wcast-qual
    -Wshadow -Wwrite-strings -Wconversion -Wunreachable-code -Wunused-macros
)
