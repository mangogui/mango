cmake_minimum_required(VERSION 3.16)
project(CoreGraphicsEngine LANGUAGES C CXX OBJC OBJCXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_SHARED_LIBS "Build shared libraries" ON)

set(SOURCES
    CoreGraphicsHelper.cpp
    CoreGraphicsContext.mm)

set(HEADERS
    CoreGraphicsHelper.h
    CoreGraphicsContext.h)

add_library(CoreGraphicsEngine STATIC ${SOURCES} ${HEADERS})
add_library(Mango::CoreGraphicsEngine ALIAS CoreGraphicsEngine)

target_link_libraries(CoreGraphicsEngine PUBLIC Mango::Core
    "-framework Cocoa"
    "-framework Metal"
    "-framework QuartzCore"
    "-framework MetalKit")

set_target_properties(CoreGraphicsEngine PROPERTIES OUTPUT_NAME "CoreGraphicsEngine" PREFIX "lib")

target_include_directories(CoreGraphicsEngine PUBLIC
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/Mango>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/Mango/widgets>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/Mango/core/animations>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/Mango/core>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/graphics/coregraphics>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/core>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/core/animations>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/plugins/platforms/windows>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/plugins/common>)

install(FILES ${HEADERS}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/Mango/widgets)

install(TARGETS CoreGraphicsEngine EXPORT MangoTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
