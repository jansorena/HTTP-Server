cmake_minimum_required(VERSION 3.10)
project(Redes-Lab2)

set(CMAKE_C_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(server src/server.cpp)
add_executable(client src/client.cpp)

include_directories(include)