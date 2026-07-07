cmake_minimum_required(VERSION 3.20)   # Исправлено: 3.20
project(MyProject)                      # Исправлено: есть имя

set(CMAKE_CXX_STANDARD 20)

add_executable(MyProgram main.cpp
        enum_col_rob.h
        enum_col_rob.cpp
        colonist.cpp
        colonist.h
        weather.h
        modules.cpp
        resourses.cpp
        resources.h
        weather.cpp
        accident.cpp
        accident.h
        modules.h
        robot.cpp
        robot.h
)      # Исправлено: есть имя программы