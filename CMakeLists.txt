cmake_minimum_required(VERSION 3.28.3 FATAL_ERROR)

set (CMAKE_CXX_STANDARD 17)


set(PROJECT_NAME BattleCity)

project(${PROJECT_NAME})

set()

add_executable(${PROJECT_NAME} src/main.cpp)

add_subdirectory(external/glfw)
target_link_libraries(${PROJECT_NAME} glfw)

add_subdirectory(external/glad)
target_link_libraries(${PROJECT_NAME} glad)

add_subdirectory(external/glm)
target_link_libraries(${PROJECT_NAME} glm)

target_link_directories(${PROJECT_NAME} PUBLIC src)

