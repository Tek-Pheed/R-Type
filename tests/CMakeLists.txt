cmake_minimum_required(VERSION 3.25.0)

project(tests
        VERSION 0.0.1
        DESCRIPTION "R-Type tests"
        LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(SFML_STATIC_LIBRARIES TRUE)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)


file(GLOB_RECURSE SRC "./src/*.cpp")
add_executable(${PROJECT_NAME} ${SRC} ${APP})
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/libs/ecs/include/)
target_link_libraries(${PROJECT_NAME} ecs)
target_link_libraries(${PROJECT_NAME} criterion)
