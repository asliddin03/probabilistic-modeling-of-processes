cmake_minimum_required(VERSION 3.16)

project(task_8 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

qt_standard_project_setup()

qt_add_executable(task_8
        main.cpp
        mainwindow.cpp
        mainwindow.h
        drawwidget.cpp
        drawwidget.h
)

target_link_libraries(task_8 PRIVATE Qt6::Widgets)