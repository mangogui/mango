cmake_minimum_required(VERSION 3.23)
project(Mango)

set(CMAKE_CXX_STANDARD 14)


if (APPLE)
    set(SOURCE_FILES
            platforms/cocoa/CocoaWindow.mm
            platforms/cocoa/CocoaApplication.mm
            Size.cpp)
    set(HEADER_FILES
            platforms/cocoa/CocoaWindow.h
            platforms/cocoa/CocoaApplication.h
            Size.h platforms/cocoa/CocoaWindowInterface.cpp platforms/cocoa/CocoaWindowInterface.h)
elseif (WIN32)
    set(SOURCE_FILES
            platforms/win32/Win32Window.cpp
            Size.cpp)
    set(HEADER_FILES
            platforms/win32/Win32Window.h
            Size.h platforms/win32/Win32Application.cpp platforms/win32/Win32Application.h platforms/Window.cpp platforms/Window.h platforms/Application.cpp platforms/Application.h platforms/platform.h)
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

