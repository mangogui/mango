cmake_minimum_required(VERSION 3.16)
project(Core VERSION ${Mango_VERSION} LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(CMakePackageConfigHelpers)

set(SOURCES
    MNSize.cpp
    MNRect.cpp
    PaintEvent.cpp
    Color.cpp
    MouseEvent.cpp
    PainterPath.cpp
    EasingCurve.cpp
    Point.cpp
    ResizeEvent.cpp
    Event.cpp
    FillStyle.cpp
)

set(HEADERS
    MNSize.h
    MNRect.h
    PaintEvent.h
    Color.h
    MouseEvent.h
    PainterPath.h
    EasingCurve.h
    ResizeEvent.h
    Event.h
    Point.h
    FillStyle.h
)

set(ANIMATION_SOURCES
    animations/NumberAnimation.cpp
    animations/ColorAnimation.cpp
    animations/VariantAnimation.cpp
    AnimationRegistry.cpp
)

set(ANIMATION_HEADERS
    animations/NumberAnimation.h
    animations/ColorAnimation.h
    animations/Variant.h
    animations/VariantAnimation.h
    AnimationRegistry.h
)

if (BUILD_MACOS)
    list(APPEND ANIMATION_SOURCES macos/AnimationScheduler.cpp)
    list(APPEND ANIMATION_HEADERS macos/AnimationScheduler.h)
elseif (BUILD_WINDOWS)
    list(APPEND ANIMATION_SOURCES windows/AnimationScheduler.cpp)
    list(APPEND ANIMATION_HEADERS windows/AnimationScheduler.h)
endif ()

add_library(Core STATIC ${SOURCES} ${HEADERS} ${ANIMATION_SOURCES} ${ANIMATION_HEADERS})
add_library(Mango::Core ALIAS Core)

set_target_properties(Core PROPERTIES
    OUTPUT_NAME "MangoCore"
)

target_include_directories(Core PUBLIC
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/Mango/core/animations>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/Mango/core>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/core>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/core/animations>
)

install(FILES ${HEADERS}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/Mango/core
)

install(FILES ${ANIMATION_HEADERS}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/Mango/core/animations
)

install(TARGETS Core EXPORT MangoTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
