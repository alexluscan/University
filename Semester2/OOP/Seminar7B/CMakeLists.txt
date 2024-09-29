cmake_minimum_required(VERSION 3.23)
project(Seminar7B)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_PREFIX_PATH /opt/homebrew/opt/qt)

find_package(Qt5 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

add_executable(Seminar7B main.cpp
        ChatSession.cpp
        ChatSession.h
        GUI.cpp
        GUI.h
        Subject.cpp
        Subject.h
        User.h
        Observer.h
        ui_GUI.h
)
target_link_libraries(Seminar7B
        Qt::Core
        Qt::Gui
        Qt::Widgets
)