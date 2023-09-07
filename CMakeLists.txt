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
            platforms/cocoa/Renderer.mm platforms/cocoa/PainterPath.cpp platforms/cocoa/PainterPath.h platforms/cocoa/Vector.cpp platforms/cocoa/Vector.h CustomWindow.cpp CustomWindow.h Mango.h platforms/cocoa/Screen.cpp platforms/cocoa/Screen.h)
    set(HEADER_FILES
            Size.h
            Rect.h
            Point.h
            platforms/cocoa/CocoaWindow.h
            platforms/cocoa/CocoaApplication.h
            platforms/cocoa/CocoaWindowInterface.h
            platforms/cocoa/Painter.mm platforms/cocoa/Painter.h
            Color.cpp
            platforms/cocoa/Renderer.h platforms/cocoa/PainterPath.cpp platforms/cocoa/PainterPath.h platforms/cocoa/Vector.cpp platforms/cocoa/Vector.h CustomWindow.cpp CustomWindow.h Mango.h platforms/cocoa/Screen.cpp platforms/cocoa/Screen.h)
elseif (WIN32)
    set(SOURCE_FILES
            platforms/win32/Win32Window.cpp
            Size.cpp Rect.cpp Rect.h Point.cpp Point.h Color.cpp Color.h)
    set(HEADER_FILES
            platforms/win32/Win32Window.h
            Size.h platforms/win32/Win32Application.cpp platforms/win32/Win32Application.h platforms/Window.cpp platforms/Window.h platforms/Application.cpp platforms/Application.h platforms/platform.h Rect.cpp Rect.h Point.cpp Point.h platforms/cocoa/Painter.cpp platforms/cocoa/Painter.h Color.cpp Color.h platforms/cocoa/PainterPath.cpp platforms/cocoa/PainterPath.h)
endif()

set(METAL_SHADERS
        platforms/cocoa/shaders/EllipseShaders.metal
        platforms/cocoa/shaders/RectShaders.metal)

# Add a custom command to compile the Metal shaders at build time
foreach(SHADER_FILE ${METAL_SHADERS})
    get_filename_component(SHADER_NAME ${SHADER_FILE} NAME_WE)
    add_custom_command(
            OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/airs/${SHADER_NAME}.air
            COMMAND xcrun -sdk macosx metal -c ${CMAKE_CURRENT_SOURCE_DIR}/${SHADER_FILE} -o ${CMAKE_CURRENT_BINARY_DIR}/airs/${SHADER_NAME}.air
            DEPENDS ${SHADER_FILE}
    )
    list(APPEND SOURCE_FILES ${CMAKE_CURRENT_BINARY_DIR}/airs/${SHADER_NAME}.air)
endforeach()

# Compile the Metal shaders to a single metallib file
add_custom_command(
        OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/shaders_lib/Shaders.metallib
        COMMAND xcrun -sdk macosx metal -frecord-sources -o ${CMAKE_CURRENT_BINARY_DIR}/Shaders.metallib ${CMAKE_CURRENT_BINARY_DIR}/airs/EllipseShaders.air ${CMAKE_CURRENT_BINARY_DIR}/airs/RectShaders.air
        DEPENDS ${METAL_SHADERS}
)

add_library(Mango SHARED ${HEADER_FILES} ${SOURCE_FILES}
        ${CMAKE_CURRENT_BINARY_DIR}/shaders_lib/Shaders.metallib)


if (APPLE)
    target_link_libraries(Mango "-framework Cocoa" "-framework Metal" "-framework QuartzCore"
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

