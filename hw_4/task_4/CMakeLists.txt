cmake_minimum_required(VERSION 3.16)
project(task_4 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

add_executable(task_4
        src/main.cpp
        core/config.cpp
        core/distributions.cpp
        core/simulation.cpp
        gui/mainwindow.cpp
)

target_include_directories(task_4 PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(task_4 PRIVATE
        Qt6::Core
        Qt6::Widgets
)