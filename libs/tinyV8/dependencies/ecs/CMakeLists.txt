cmake_minimum_required(VERSION 3.25.0)

project(ecs
    VERSION 0.0.1
    DESCRIPTION "R-Type Entity Component System"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(SFML_STATIC_LIBRARIES TRUE)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
set(CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION "10.0")

file(GLOB_RECURSE SRC "./src/*.cpp")
file(GLOB_RECURSE APP "./include/*.hpp")

add_library(${PROJECT_NAME} STATIC ${SRC} ${APP})
target_include_directories(${PROJECT_NAME} PUBLIC include)

target_link_libraries(${PROJECT_NAME} sfml-system sfml-window sfml-graphics sfml-audio)

# set_target_properties(${PROJECT_NAME} PROPERTIES
# INSTALL_RPATH $<TARGET_FILE_DIR:${PROJECT_NAME}>
# BUILD_WITH_INSTALL_RPATH TRUE
# CMAKE_BUILD_RPATH $<TARGET_FILE_DIR:${PROJECT_NAME}>
# )

if(WIN32)
    add_definitions(-DWIN32)
else()
    add_definitions(-DLINUX)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g3 -ggdb")
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Werror
        -Wswitch-default -Wcast-align -Wpointer-arith -Wundef -Wcast-qual
        -Wshadow -Wwrite-strings -Wconversion -Wunreachable-code -Wunused-macros
        $<$<CONFIG:Debug>:-g3>
    )
endif()