cmake_minimum_required(VERSION 3.25.0)

project(game
    VERSION 0.0.1
    DESCRIPTION "R-Type client"
    LANGUAGES CXX
)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g3 -ggdb")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION "10.0")

file(GLOB_RECURSE SRC "./src/*.cpp")
file(GLOB_RECURSE APP "./include/*.hpp")

add_executable(client ${SRC} ${APP})
target_include_directories(client PRIVATE include)
target_link_libraries(client ecs)
target_link_libraries(client ossystem)

add_executable(server ${SRC} ${APP})
target_include_directories(server PRIVATE include)
target_link_libraries(server ecs)
target_link_libraries(server ossystem)

set_target_properties(client PROPERTIES
    INSTALL_RPATH $<TARGET_FILE_DIR:${PROJECT_NAME}>
    BUILD_WITH_INSTALL_RPATH TRUE
    CMAKE_BUILD_RPATH $<TARGET_FILE_DIR:${PROJECT_NAME}>
)

set_target_properties(server PROPERTIES
    INSTALL_RPATH $<TARGET_FILE_DIR:${PROJECT_NAME}>
    BUILD_WITH_INSTALL_RPATH TRUE
    CMAKE_BUILD_RPATH $<TARGET_FILE_DIR:${PROJECT_NAME}>
)

include_directories(../libs/system/include)
include_directories(../libs/ecs/include)

target_compile_definitions(server PRIVATE -DRTYPE_SERVER)

if(WIN32)
    add_definitions(-DWIN32)
else()
    add_definitions(-DLINUX)
    target_link_libraries(server pthread)
    target_link_libraries(client pthread)
    target_compile_options(server PRIVATE -Wall -Wextra -Werror
        -Wswitch-default -Wcast-align -Wpointer-arith -Wundef -Wcast-qual
        -Wshadow -Wwrite-strings -Wconversion -Wunreachable-code -Wunused-macros
        $<$<CONFIG:Debug>:-g3>
    )
    target_compile_options(client PRIVATE -Wall -Wextra -Werror
        -Wswitch-default -Wcast-align -Wpointer-arith -Wundef -Wcast-qual
        -Wshadow -Wwrite-strings -Wconversion -Wunreachable-code -Wunused-macros
        $<$<CONFIG:Debug>:-g3>
    )
endif()