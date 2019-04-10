cmake_minimum_required(VERSION 3.10)
project(twin)

find_package(SDL2 REQUIRED)

file(GLOB SRC "twin/*.hpp" "twin/*.cpp")
add_executable(${PROJECT_NAME} ${SRC})
target_link_libraries(${PROJECT_NAME} SDL2)
