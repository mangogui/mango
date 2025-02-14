cmake_minimum_required(VERSION 3.16)
project(MangoWidgets LANGUAGES C CXX OBJC OBJCXX)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES
    MNSize.cpp
    MNRect.cpp
    PaintEvent.cpp
    Color.cpp
    MouseEvent.cpp
    PainterPath.cpp
    EasingCurve.cpp
    NumberAnimation.cpp
    ColorAnimation.cpp
    VariantAnimation.cpp
    Widget.mm
    CocoaWindowObjC.mm
    WindowDelegate.mm
    ViewObjC.mm
    Application.mm
    Canvas.mm
    Point.cpp
    ResizeEvent.cpp
    Point.cpp
)

set(HEADERS
    MNSize.h
    MNRect.h
    PaintEvent.h
    Application.h
    Color.h
    Widget.h
    MouseEvent.h
    WindowDelegate.h
    ViewObjC.h
    CocoaWindowObjC.h
    PainterPath.h
    Canvas.h
    EasingCurve.h
    NumberAnimation.h
    ColorAnimation.h
    Variant.h
    VariantAnimation.h
    ResizeEvent.h
    Point.h
)

# Define the shared library
add_library(Widgets SHARED ${SOURCES} ${HEADERS})

# Create an alias for Mango::Widgets
add_library(Mango::Widgets ALIAS Widgets)

# Set properties for the target
set_target_properties(Widgets PROPERTIES
    OUTPUT_NAME "MangoWidgets"
)

target_link_libraries(Widgets
    "-framework Cocoa"
    "-framework Metal"
    "-framework QuartzCore"
    "-framework MetalKit"
)

# Update library output path to use top-level build directory
set(LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}/libs")

# Correct include directories for both build and install phases
target_include_directories(Widgets PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/Mango>
)

# Install headers to Mango subdirectory under include
install(FILES ${HEADERS}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/Mango
)

# Install the target and create an export set
install(TARGETS Widgets
    EXPORT MangoTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# Export the MangoWidgets library for CMake find_package
install(EXPORT MangoTargets
    FILE MangoWidgetsConfig.cmake
    NAMESPACE Mango::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Mango
)