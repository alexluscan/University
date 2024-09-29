cmake_minimum_required(VERSION 3.23)
project(shopping_list)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_PREFIX_PATH /opt/homebrew/opt/qt)

find_package(Qt5 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

add_executable(shopping_list main.cpp item.h repository.cpp repository.h service.cpp service.h gui.cpp gui.h)
target_link_libraries(shopping_list
        Qt::Core
        Qt::Gui
        Qt::Widgets
        )

