cmake_minimum_required(VERSION 3.25.0)

project(ecs
    VERSION 0.0.1
    DESCRIPTION "R-Type Entity Component System"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(SFML_STATIC_LIBRARIES TRUE)
set(BUILD_SHARED_LIBS OFF)

file(GLOB_RECURSE SRC "./src/*.cpp")
file(GLOB_RECURSE APP "./include/*.hpp")

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
add_library(${PROJECT_NAME} SHARED ${SRC} ${APP})
target_include_directories(${PROJECT_NAME} PRIVATE include)

target_link_libraries(${PROJECT_NAME} sfml-system sfml-window sfml-graphics)

if (WIN32)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -WX
    )
else ()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Werror
    -Wswitch-default -Wcast-align -Wpointer-arith -Wundef -Wcast-qual
    -Wshadow -Wwrite-strings -Wconversion -Wunreachable-code -Wunused-macros
)
endif()