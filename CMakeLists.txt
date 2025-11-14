cmake_minimum_required(VERSION 2.8)

project(Lite3_udp)

set(CMAKE_BUILD_TYPE "Release")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# For test
add_executable(simple_cmd test/simple_cmd.cpp)
add_executable(receive_state test/RobotState.cpp)
add_executable(complex_cmd test/complex_cmd.cpp)

add_library(sender SHARED src/sender.cpp)
add_executable(lite3_command src/lite3_command.cpp)
target_link_libraries(lite3_command sender)

