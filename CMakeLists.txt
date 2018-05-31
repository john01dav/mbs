cmake_minimum_required(VERSION 3.10)
project(mbs)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(yaml-cpp)
include_directories(yaml-cpp/include)

add_library(libmbs STATIC modules/libmbs/source/MbsProject.cpp modules/libmbs/include/MbsProject.h)
add_executable(mbs modules/mbs/source/main.cpp)
target_link_libraries(mbs libmbs yaml-cpp)
include_directories(modules/libmbs/include modules/mbs/include)