cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_SHARED_LIBS "Build shared libraries" ON)

set(SOURCES
    Application.cpp
    Win32Window.cpp
    Win32View.cpp
    WindowProcedure.cpp)

set(HEADERS
    Application.h
    Win32Window.h
    Win32View.h
    WindowProcedure.h)

add_library(Widgets STATIC ${SOURCES} ${HEADERS})
add_library(Mango::Widgets ALIAS Widgets)

target_link_libraries(Widgets PUBLIC Mango::Core Mango::Common Mango::Direct2DGraphicsEngine d2d1 dwrite winmm)

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
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/plugins/common>
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
