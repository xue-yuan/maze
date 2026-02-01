cmake_minimum_required(VERSION 3.10)
project(Maze)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

file(GLOB SOURCES "src/*.cc")
add_executable(maze ${SOURCES})
