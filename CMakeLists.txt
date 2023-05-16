cmake_minimum_required(VERSION 3.16)
project(intrinsics_detection)
#set(CMAKE_OSX_ARCHITECTURES "x86_64") #for testing x86_64 on my mac
set(CMAKE_CXX_STANDARD 17)

add_executable(get_intrin main.cpp)