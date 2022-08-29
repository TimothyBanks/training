cmake_minimum_required (VERSION 3.8)
project (TRAINING)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable (dapperlabs src/main.cpp)
target_include_directories (dapperlabs PUBLIC ./include)