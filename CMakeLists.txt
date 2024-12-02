cmake_minimum_required(VERSION 3.25.0)

project(R-Type
    VERSION 0.0.1
    DESCRIPTION "R-Type Project"
    LANGUAGES CXX
)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g3 -ggdb")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(RELEASE_DIR ${PROJECT_SOURCE_DIR}/release)

file(MAKE_DIRECTORY ${RELEASE_DIR})

add_subdirectory(${PROJECT_SOURCE_DIR}/dependencies/SFML)
add_subdirectory(libs/ecs)
add_subdirectory(libs/system)
add_subdirectory(server)
add_subdirectory(client)

set(CLIENT_BINARY $<TARGET_FILE:client>)
set(SERVER_BINARY $<TARGET_FILE:server>)
set(ECS_LIBRARY $<TARGET_FILE:ecs>)
set(SYSTEM_LIBRARY $<TARGET_FILE:ossystem>)

add_custom_target(copy_binaries ALL
    COMMENT "Copying binaries to release folder"
)

add_custom_command(TARGET copy_binaries POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy ${CLIENT_BINARY} ${RELEASE_DIR}
    COMMAND ${CMAKE_COMMAND} -E copy ${SERVER_BINARY} ${RELEASE_DIR}
    COMMAND ${CMAKE_COMMAND} -E copy ${ECS_LIBRARY} ${RELEASE_DIR}
    COMMAND ${CMAKE_COMMAND} -E copy ${SYSTEM_LIBRARY} ${RELEASE_DIR}
)
