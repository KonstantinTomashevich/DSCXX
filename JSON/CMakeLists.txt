cmake_minimum_required(VERSION 3.15)
project(Source)

set(CMAKE_CXX_STANDARD 17)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Source)
add_subdirectory(Source)
add_subdirectory(Test)
