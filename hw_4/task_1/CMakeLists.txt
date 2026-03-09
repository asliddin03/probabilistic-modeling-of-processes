cmake_minimum_required(VERSION 3.16)

project(task_1 VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Charts)

qt_add_executable(task_1
        src/main.cpp
        core/simulation.cpp
        core/simulation.h
        gui/mainwindow.cpp
        gui/mainwindow.h
)

target_include_directories(task_1 PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/core
        ${CMAKE_CURRENT_SOURCE_DIR}/gui
)

target_link_libraries(task_1 PRIVATE
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
        Qt6::Charts
)