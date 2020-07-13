# /////////////////////////////////////////////////////////////////////////////
# // Name:        cmake configuration
# // Purpose:     messageQueue: extremely simple cmake
# // Author:      Mohd Radzi Ibrahim
# // Modified by:
# // Created:     13 July 2020
# // Copyright:   (c) MR Ibrahim
# // Licence:     GNU GENERAL PUBLIC LICENSE
# /////////////////////////////////////////////////////////////////////////////



cmake_minimum_required(VERSION 3.17)
project(signal)
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_executable(${PROJECT_NAME} example.cpp)
target_link_libraries(${PROJECT_NAME} Threads::Threads)
target_compile_features(signal PUBLIC cxx_std_17)
