cmake_minimum_required(VERSION 3.00)
project(PackupC)
set(CMAKE_CXX_STANDARD 20)

add_subdirectory(yaml-cpp)
#include_directories(yaml-cpp/includes)

add_executable(PackupC src/main.cpp src/parser.cpp src/action.cpp src/parser.hpp src/action.hpp)

target_link_libraries(PackupC PRIVATE yaml-cpp::yaml-cpp)


