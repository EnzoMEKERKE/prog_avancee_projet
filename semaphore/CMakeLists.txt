cmake_minimum_required(VERSION 3.26)
project(semaphore)

set(CMAKE_CXX_STANDARD 20)

include_directories(headers)

add_executable(semaphore
        semaphore.cpp)
