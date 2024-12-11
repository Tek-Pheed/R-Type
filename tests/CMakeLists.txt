cmake_minimum_required(VERSION 3.25.0)

project(tests
        VERSION 0.0.1
        DESCRIPTION "R-Type tests"
        LANGUAGES CXX
)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g3 -ggdb")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION "10.0")

file(GLOB_RECURSE SRC "./src/*.cpp")
file(GLOB_RECURSE APP "./include/*.hpp")

add_executable(${PROJECT_NAME} ${SRC} ${APP})
target_include_directories(${PROJECT_NAME} PRIVATE include)
target_link_libraries(${PROJECT_NAME} ecs)
include_directories(../libs/ecs/include)
target_link_libraries(${PROJECT_NAME} criterion)

set_target_properties(${PROJECT_NAME} PROPERTIES
        INSTALL_RPATH $<TARGET_FILE_DIR:${PROJECT_NAME}>:$ENV{LD_LIBRARY_PATH}
        BUILD_WITH_INSTALL_RPATH TRUE
        CMAKE_BUILD_RPATH $<TARGET_FILE_DIR:${PROJECT_NAME}>:$ENV{LD_LIBRARY_PATH}
)
