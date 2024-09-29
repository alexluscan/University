cmake_minimum_required(VERSION 3.23)
project(Weather)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_PREFIX_PATH /opt/homebrew/opt/qt)

find_package(Qt5 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

add_executable(Weather main.cpp item.h repository.cpp repository.h service.cpp service.h gui.cpp gui.h)
target_link_libraries(Weather
        Qt::Core
        Qt::Gui
        Qt::Widgets
        )

