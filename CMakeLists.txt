cmake_minimum_required(VERSION 3.10)
project(mbs)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(yaml-cpp)
include_directories(yaml-cpp/include)

include(mbs-cmake.cmake)
target_link_libraries(module-mbs yaml-cpp stdc++fs)

#add_library(libmbs STATIC modules/libmbs/source/MbsProject.cpp modules/libmbs/include/mbs/MbsProject.h modules/libmbs/include/mbs/MbsException.h modules/libmbs/include/mbs/Module.h modules/libmbs/source/Module.cpp modules/libmbs/source/ApplicationModule.cpp modules/libmbs/include/mbs/ApplicationModule.h modules/libmbs/source/LibraryModule.cpp modules/libmbs/include/mbs/LibraryModule.h)
#add_executable(mbs modules/mbs/source/main.cpp)
#target_link_libraries(mbs libmbs yaml-cpp stdc++fs)
#include_directories(modules/libmbs/include modules/mbs/include)