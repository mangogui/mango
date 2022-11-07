cmake_minimum_required(VERSION 3.23)
project(Mango)

set(CMAKE_CXX_STANDARD 14)


if (APPLE)
    set(SOURCE_FILES
            CocoaWindow.mm
            CocoaApplication.mm
            Size.cpp)
    set(HEADER_FILES
            CocoaWindow.h
            CocoaApplication.h
            Size.h CocoaWindowInterface.cpp CocoaWindowInterface.h)
elseif (WIN32)
    set(SOURCE_FILES
            Win32Window.cpp
            Size.cpp)
    set(HEADER_FILES
            Win32Window.h
            Size.h Win32Application.cpp Win32Application.h)
endif()

add_library(Mango SHARED ${HEADER_FILES} ${SOURCE_FILES})

if (APPLE)
    target_link_libraries(Mango "-framework Cocoa")
elseif (WIN32)
    target_link_libraries(Mango)
endif()

if (WIN32)
    set(SUBSYSTEM_LINKER_OPTIONS "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
endif()

link_libraries(Mango)
add_executable(main main.cpp)

