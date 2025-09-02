cmake_minimum_required(VERSION 4.1.0 FATAL_ERROR)

set (CMAKE_CXX_STANDARD 17)


set(PROJECT_NAME BattleCity)

project(${PROJECT_NAME})

add_executable(${PROJECT_NAME} src/main.cpp)

add_subdirectory(external/glfw)
target_link_libraries(${PROJECT_NAME} glfw)

add_subdirectory(external/glad)
target_link_libraries(${PROJECT_NAME} glad)

add_subdirectory(external/glm)
target_link_libraries(${PROJECT_NAME} glm)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}")

file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/includes/" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/includes")
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/shaders/" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/shaders")
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/textures/" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/textures")
