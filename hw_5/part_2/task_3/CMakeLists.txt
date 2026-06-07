cmake_minimum_required(VERSION 3.16)

project(task_3 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(task_3
        main.cpp
        models.h
        file_reader.cpp
        cipher.cpp
        probability.cpp
        printer.cpp
        file_reader.cpp
        file_reader.h
        cipher.cpp
        cipher.h
        probability.cpp
        probability.h
        printer.cpp
        printer.h
)