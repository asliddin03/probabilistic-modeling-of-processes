cmake_minimum_required(VERSION 3.16)

project(task5 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC OFF)
set(CMAKE_AUTORCC OFF)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(task5
        src/main.cpp
        core/config.cpp
        core/distributions.cpp
        core/simulation.cpp
        gui/plotwidget.cpp
        gui/mainwindow.cpp
)

target_include_directories(task5 PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/core
        ${CMAKE_CURRENT_SOURCE_DIR}/gui
)

target_link_libraries(task5 PRIVATE Qt6::Widgets)