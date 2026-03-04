cmake_minimum_required(VERSION 3.20)
project(part_1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(task_1 task_1/main.cpp)
add_executable(task_3
        task_3/src/main.cpp
        task_3/src/input.cpp
        task_3/src/sim.cpp
        task_3/src/output.cpp
)
#cmake -S . -B build
#cmake --build build
#./build/task_3 2 1 2 4 4 3 3 10 10 4 4