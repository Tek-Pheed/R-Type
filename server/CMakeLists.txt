cmake_minimum_required(VERSION 3.25.0)

project(server
    VERSION 0.0.1
    DESCRIPTION "R-Type server"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB_RECURSE SRC "./src/*.cpp")
file(GLOB_RECURSE APP "./include/*.hpp")

add_executable(${PROJECT_NAME} ${SRC} ${APP})
target_include_directories(${PROJECT_NAME} PRIVATE include)
target_link_libraries(${PROJECT_NAME} ecs)
target_link_libraries(${PROJECT_NAME} system)

if (WIN32)
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -WX
    )
else ()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Werror
    -Wswitch-default -Wcast-align -Wpointer-arith -Wundef -Wcast-qual
    -Wshadow -Wwrite-strings -Wconversion -Wunreachable-code -Wunused-macros
)
endif()