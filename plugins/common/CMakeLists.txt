cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES
    PlatformView.cpp
    PlatformWindow.cpp
    GraphicsContext.cpp
    Object.cpp
    WindowManager.cpp
    Canvas.cpp
)

set(HEADERS
    PlatformView.h
    PlatformWindow.h
    GraphicsContext.h
    Object.h
    WindowManager.h
    Canvas.h
    AbstractWidget.h
)

add_library(Common STATIC ${SOURCES} ${HEADERS})
add_library(Mango::Common ALIAS Common)

target_link_libraries(Common PUBLIC Mango::Core)

set_target_properties(Common PROPERTIES OUTPUT_NAME "MangoCommon" PREFIX "lib")

target_include_directories(Common PUBLIC
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/Mango>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/Mango/widgets>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/Mango/core/animations>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/Mango/core>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/plugins/common>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/core>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/core/animations>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/core/macos>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/graphics/coregraphics>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/plugins/platforms/macos>
)

install(FILES ${HEADERS}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/Mango/widgets
)

install(TARGETS Common EXPORT MangoTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
