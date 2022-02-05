cmake_minimum_required(VERSION 3.21)
project(ObjectTracking)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


find_package(Qt6 COMPONENTS
        Core
        REQUIRED)
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})


add_executable(ObjectTracking main.cpp ImageProcess.cpp ImageProcess.h Racer.cpp Racer.h)
target_link_libraries(ObjectTracking
        Qt::Core
        /usr/local/lib/libuvc.so.0
        ${OpenCV_LIBS}
        )

