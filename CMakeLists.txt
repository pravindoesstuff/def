cmake_minimum_required(VERSION 3.22)
project(def C)

set(CMAKE_C_STANDARD 99)
find_package(CURL REQUIRED)
set(CMAKE_C_FLAGS "-lcurl")
add_executable(def main.c)
