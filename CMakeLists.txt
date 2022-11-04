cmake_minimum_required(VERSION 3.23)
project(Mango)

set(CMAKE_CXX_STANDARD 14)

find_package(PythonLibs REQUIRED)
include_directories(${PYTHON_INCLUDE_DIRS})

set(SOURCE_FILES
        CocoaWindow.mm
        CocoaApplication.mm
        Size.cpp)

set(HEADER_FILES
        CocoaWindow.h
        CocoaApplication.h
        Size.h CocoaWindowInterface.cpp CocoaWindowInterface.h)

add_library(Mango SHARED ${HEADER_FILES} ${SOURCE_FILES})

target_link_libraries(Mango "-framework Cocoa" ${PYTHON_LIBRARIES})

link_libraries(Mango)

add_executable(main main.cpp)

