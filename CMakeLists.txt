cmake_minimum_required(VERSION 3.26)
project(hw8)

set(CMAKE_CXX_STANDARD 17)

add_executable(hw8 main.cpp)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(hw8 Threads::Threads)
