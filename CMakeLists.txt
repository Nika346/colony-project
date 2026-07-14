cmake_minimum_required(VERSION 3.20)   # Исправлено: 3.20
project(MyProject)                      # Исправлено: есть имя

set(CMAKE_CXX_STANDARD 20)

add_executable(MyProgram main.cpp
        enum_col_rob.h
        enum_col_rob.cpp
        colonist.cpp
        colonist.h
        Weather.h
        modules.cpp
        resources.cpp
        resources.h
        Weather.cpp
        accident.cpp
        accident.h
        modules.h
        robot.cpp
        robot.h
        Colony.h
        Colony.cpp
        TransportRoute.cpp
        TransportRoute.h
        PriorityTaskQueue.cpp
        PriorityTaskQueue.h
        Task.cpp
        Task.h
        main.cpp
)      # Исправлено: есть имя программы