cmake_minimum_required(VERSION 2.8)

project(Deeprobotics_udp)

set(CMAKE_BUILD_TYPE "Release")

add_executable(simple_cmd src/simple_cmd.cpp)
add_executable(receive_state src/RobotState.cpp)

