cmake_minimum_required(VERSION 3.16)
project(Direct2DGraphicsEngine LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_SHARED_LIBS "Build shared libraries" ON)

set(SOURCES
    D2D1Helpers.cpp
    Direct2DGraphicsContext.cpp)

set(HEADERS
    D2D1Helpers.h
    Direct2DGraphicsContext.h)

add_library(Direct2DGraphicsEngine STATIC ${SOURCES} ${HEADERS})
add_library(Mango::Direct2DGraphicsEngine ALIAS Direct2DGraphicsEngine)

target_link_libraries(Direct2DGraphicsEngine PUBLIC Mango::Core d2d1 dwrite winmm)
set_target_properties(Direct2DGraphicsEngine PROPERTIES OUTPUT_NAME "Direct2DGraphicsEngine" PREFIX "lib")

target_include_directories(Direct2DGraphicsEngine PUBLIC
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/Mango>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/Mango/widgets>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/Mango/core/animations>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/Mango/core>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/graphics/direct2d>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/core>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/core/animations>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/plugins/platforms/windows>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/plugins/platforms/common>)

install(FILES ${HEADERS}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/Mango/widgets)

install(TARGETS Direct2DGraphicsEngine EXPORT MangoTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
