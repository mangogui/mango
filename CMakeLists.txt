cmake_minimum_required(VERSION 3.23)
project(Mango)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_C_FLAGS "-Wdeprecated-declarations")

if (APPLE)
    set(SOURCE_FILES
            Size.cpp
            Rect.cpp
            Point.cpp
            platforms/cocoa/CocoaWindow.mm
            platforms/cocoa/CocoaApplication.mm
            platforms/cocoa/CocoaWindowInterface.cpp
            platforms/cocoa/Painter.mm platforms/cocoa/Painter.h
            Color.cpp
            )
    set(HEADER_FILES
            Size.h
            Rect.h
            Point.h
            platforms/cocoa/CocoaWindow.h
            platforms/cocoa/CocoaApplication.h
            platforms/cocoa/CocoaWindowInterface.h
            platforms/cocoa/Painter.mm platforms/cocoa/Painter.h
            Color.cpp
            )
elseif (WIN32)
    set(SOURCE_FILES
            platforms/win32/Win32Window.cpp
            Size.cpp Rect.cpp Rect.h Point.cpp Point.h Color.cpp Color.h)
    set(HEADER_FILES
            platforms/win32/Win32Window.h
            Size.h platforms/win32/Win32Application.cpp platforms/win32/Win32Application.h platforms/Window.cpp platforms/Window.h platforms/Application.cpp platforms/Application.h platforms/platform.h Rect.cpp Rect.h Point.cpp Point.h platforms/cocoa/Painter.mm platforms/cocoa/Painter.h Color.cpp Color.h)
endif()

add_library(Mango SHARED ${HEADER_FILES} ${SOURCE_FILES})

if (APPLE)
    target_link_libraries(Mango "-framework Cocoa" "-framework Metal"
            "-framework MetalKit" ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})
elseif (WIN32)
    find_package(OpenGL REQUIRED)
    find_package(GLUT REQUIRED)
    include_directories( ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} )
    target_link_libraries(Mango ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})
endif()

if (WIN32)
    set(SUBSYSTEM_LINKER_OPTIONS "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
endif()


link_libraries(Mango)
add_executable(main main.cpp)

