cmake_minimum_required(VERSION 3.10)
project(hw_10)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

add_executable(hw_10
        src/main.cpp
        src/graph.cpp
        src/stats.cpp
        src/random_graph.cpp
        src/components.cpp
        src/mst.cpp
        src/cycles.cpp
        src/trees.cpp
)