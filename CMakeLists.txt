cmake_minimum_required(VERSION 3.21)
project(Rename)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_PREFIX_PATH "/home/alex/Qt/6.2.2/gcc_64")

include_directories(./)

find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

add_executable(Rename main.cpp MainWindow.cpp MainWindow.h MainWindow.ui ThreadReName.cpp ThreadReName.h)
target_link_libraries(Rename
        Qt::Core
        Qt::Gui
        Qt::Widgets
        )

