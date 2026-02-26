cmake_minimum_required(VERSION 3.16)
project(RumorSimQt LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

qt_standard_project_setup()

add_executable(RumorSimQt
        main.cpp
        MainWindow.h MainWindow.cpp
        RumorSimulator.h RumorSimulator.cpp
)

target_link_libraries(RumorSimQt PRIVATE Qt6::Widgets)