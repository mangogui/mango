cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES
    Application.mm
    WindowDelegate.mm
    ViewObjC.mm
    CocoaWindowObjC.mm
    CocoaView.mm
    CocoaWindow.mm
)

set(HEADERS
    Application.h
)

add_library(Platform STATIC ${SOURCES} ${HEADERS})
add_library(Mango::Platform ALIAS Platform)

target_link_libraries(Platform PUBLIC Mango::Core
    Mango::CoreGraphicsEngine
    Mango::Common
    "-framework Cocoa"
    "-framework Metal"
    "-framework QuartzCore"
    "-framework MetalKit")

set_target_properties(Platform PROPERTIES OUTPUT_NAME "MangoPlatform" PREFIX "lib")

target_include_directories(Platform PUBLIC
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/Mango>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/Mango/widgets>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/Mango/core/animations>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/Mango/core>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/plugins/common>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/core>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/widgets>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/core/animations>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/core/macos>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/graphics/coregraphics>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/plugins/platforms/macos>
)

install(TARGETS Platform EXPORT MangoTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)