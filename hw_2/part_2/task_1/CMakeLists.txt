cmake_minimum_required(VERSION 3.16)

project(task_1 VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(task_1
        main.cpp
        mainwindow.cpp
        epidemiclogic.cpp
)

target_link_libraries(task_1 PRIVATE Qt6::Widgets)