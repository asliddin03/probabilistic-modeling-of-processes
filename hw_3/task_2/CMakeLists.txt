cmake_minimum_required(VERSION 3.20)
project(task_2 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC OFF)
set(CMAKE_AUTORCC OFF)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(task_2
        src/main.cpp

        core/simulator.cpp

        ui/tree_renderer.cpp
        ui/mainwindow.cpp
)

target_include_directories(task_2 PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(task_2 PRIVATE Qt6::Widgets)