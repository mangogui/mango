cmake_minimum_required(VERSION 3.16)
project(Core VERSION ${Mango_VERSION} LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(CMakePackageConfigHelpers)

set(SOURCES
    Color.cpp
    PainterPath.cpp
    FillStyle.cpp


)

set(HEADERS
    Color.h
    PainterPath.h
    FillStyle.h
)

set(GEOMETRY_SOURCES
    geometry/MNSize.cpp
    geometry/MNRect.cpp
    geometry/Point.cpp
)

set(GEOMETRY_HEADERS
    geometry/MNSize.h
    geometry/MNRect.h
    geometry/Point.h
)

set(EVENT_SOURCES
    events/Event.cpp
    events/MouseEvent.cpp
    events/PaintEvent.cpp
    events/ResizeEvent.cpp
)

set(EVENT_HEADERS
    events/Event.h
    events/MouseEvent.h
    events/PaintEvent.h
    events/ResizeEvent.h)

set(ANIMATION_SOURCES
    animations/EasingCurve.cpp
    animations/NumberAnimation.cpp
    animations/ColorAnimation.cpp
    animations/VariantAnimation.cpp
    AnimationRegistry.cpp
)

set(ANIMATION_HEADERS
    animations/EasingCurve.h
    animations/NumberAnimation.h
    animations/ColorAnimation.h
    animations/Variant.h
    animations/VariantAnimation.h
    AnimationRegistry.h
)

set(LAYOUT_SOURCES
    layouts/Layout.cpp
    layouts/BoxLayout.cpp
    layouts/GridLayout.cpp
    layouts/AnchorLayout.cpp
)

set(LAYOUT_HEADERS
    layouts/Layout.h
    layouts/BoxLayout.h
    layouts/GridLayout.h
    layouts/AnchorLayout.h
)

if (BUILD_MACOS)
    list(APPEND ANIMATION_SOURCES macos/AnimationScheduler.cpp)
    list(APPEND ANIMATION_HEADERS macos/AnimationScheduler.h)
elseif (BUILD_WINDOWS)
    list(APPEND ANIMATION_SOURCES windows/AnimationScheduler.cpp)
    list(APPEND ANIMATION_HEADERS windows/AnimationScheduler.h)
endif ()

add_library(Core STATIC ${SOURCES} ${HEADERS}
    ${GEOMETRY_SOURCES} ${GEOMETRY_HEADERS}
    ${EVENT_SOURCES} ${EVENT_HEADERS}
    ${ANIMATION_SOURCES} ${ANIMATION_HEADERS}
    ${LAYOUT_SOURCES} ${LAYOUT_HEADERS})

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
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/core/events>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/core/geometry>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/core/layouts>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/plugins/common>
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
