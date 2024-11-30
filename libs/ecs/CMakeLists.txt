cmake_minimum_required(VERSION 3.25.0)

project(ecs
    VERSION 0.0.1
    DESCRIPTION "R-Type Entity Component System"
    LANGUAGES CXX
)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g3 -ggdb")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(SFML_STATIC_LIBRARIES TRUE)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

file(GLOB_RECURSE SRC "./src/*.cpp")
file(GLOB_RECURSE APP "./include/*.hpp")

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
add_library(${PROJECT_NAME} SHARED ${SRC} ${APP})
target_include_directories(${PROJECT_NAME} PRIVATE include)

target_link_libraries(${PROJECT_NAME} sfml-system sfml-window sfml-graphics)

if(WIN32)
    add_definitions(-DWIN32)
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -WX
    )
else()
    add_definitions(-DLINUX)
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Werror
        -Wswitch-default -Wcast-align -Wpointer-arith -Wundef -Wcast-qual
        -Wshadow -Wwrite-strings -Wconversion -Wunreachable-code -Wunused-macros
        $<$<CONFIG:Debug>:-g3>
    )
endif()