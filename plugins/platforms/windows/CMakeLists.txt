cmake_minimum_required(VERSION 3.16)
project(MangoWidgets LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_SHARED_LIBS "Build shared libraries" ON)

set(SOURCES
    Application.cpp
    Canvas.cpp
    WindowManager.cpp
    Win32Window.cpp
    Object.cpp
    ../common/GraphicsContext.cpp
    ../common/PlatformWindow.cpp
    ../common/Widget.cpp)

set(HEADERS
    Application.h
    Canvas.h
    WindowManager.h
    Win32Window.h
    Object.h
    ../common/GraphicsContext.h
    ../common/PlatformWindow.h
    ../common/Widget.h)

add_library(Widgets STATIC ${SOURCES} ${HEADERS})
add_library(Mango::Widgets ALIAS Widgets)

target_link_libraries(Widgets PUBLIC Mango::Core Mango::Direct2DGraphicsEngine d2d1 dwrite winmm)

set_target_properties(Widgets PROPERTIES OUTPUT_NAME "MangoWidgets" PREFIX "lib")

target_include_directories(Widgets PUBLIC
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/Mango>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/Mango/widgets>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/Mango/core/animations>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/Mango/core>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/graphics/direct2d>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/core>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/core/windows>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/core/animations>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/plugins/platforms/windows>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/plugins/platforms/common>
)

install(FILES ${HEADERS}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/Mango/widgets
)

install(TARGETS Widgets EXPORT MangoTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
