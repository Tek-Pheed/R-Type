cmake_minimum_required(VERSION 3.25.0)

project(R-Type
    VERSION 0.0.1
    DESCRIPTION "R-Type Project"
    LANGUAGES CXX
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(${PROJECT_SOURCE_DIR}/dependencies/SFML)

add_subdirectory(libs/ecs)
# add_subdirectory(client)
# add_subdirectory(tests)
