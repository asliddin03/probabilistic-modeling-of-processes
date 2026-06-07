cmake_minimum_required(VERSION 3.16)

project(hw_9 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Charts)

add_executable(hw_9
        main.cpp
        mainwindow.cpp
        discreterandomvariable.cpp
        chartbuilder.cpp
)

target_link_libraries(hw_9 PRIVATE
        Qt6::Widgets
        Qt6::Charts
)