cmake_minimum_required(VERSION 3.16)
project(MangoWidgets LANGUAGES C CXX OBJC OBJCXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES
    Widget.mm
    CocoaWindowObjC.mm
    WindowDelegate.mm
    ViewObjC.mm
    Application.mm
    Canvas.mm
)

set(HEADERS
    Application.h
    Widget.h
    WindowDelegate.h
    ViewObjC.h
    CocoaWindowObjC.h
    Canvas.h
)

add_library(Widgets STATIC ${SOURCES} ${HEADERS})
add_library(Mango::Widgets ALIAS Widgets)

target_link_libraries(Widgets PUBLIC Mango::Core
    "-framework Cocoa"
    "-framework Metal"
    "-framework QuartzCore"
    "-framework MetalKit")

set_target_properties(Widgets PROPERTIES OUTPUT_NAME "MangoWidgets" PREFIX "lib")

target_include_directories(Widgets PUBLIC
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/Mango>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/Mango/widgets>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/Mango/core/animations>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/Mango/core>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/core>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/core/animations>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/plugins/platforms/macos>
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
