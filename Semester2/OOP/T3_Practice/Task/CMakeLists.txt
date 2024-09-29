cmake_minimum_required(VERSION 3.28)
project(Task)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_PREFIX_PATH /opt/homebrew/opt/qt)
find_package(Qt5 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

add_executable(Task main.cpp
        domain.cpp
        domain.h
        repository.cpp
        repository.h
        gui.cpp
        gui.h
        service.cpp
        service.h)

target_link_libraries(Task
        Qt::Core
        Qt::Gui
        Qt::Widgets)