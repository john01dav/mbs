cmake_minimum_required(VERSION 3.5)
project(mbs)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(yaml-cpp)
include_directories(yaml-cpp/include)

include(mbs-cmake.cmake)
target_link_libraries(module-libmbs-static yaml-cpp stdc++fs)