# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES
    MNSize.h
    MNSize.cpp
    MNRect.h
    MNRect.cpp
    PaintEvent.h
    PaintEvent.cpp
    MNPoint.h
    MNPoint.cpp
    Application.h
    Application.mm
    Color.h
    Color.cpp
    Widget.h
    Widget.mm
)

add_library(Widgets SHARED
    ${SOURCES})

target_link_libraries(Widgets "-framework Cocoa" "-framework Metal" "-framework QuartzCore"
    "-framework MetalKit")

set(LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}/macos/libs")
set(Widgets_INCLUDE_DIRS "${CMAKE_INSTALL_PREFIX}/include")

install(FILES
    MNSize.h
    MNRect.h
    PaintEvent.h
    MNPoint.h
    Application.h
    Color.h
    Widget.h
    DESTINATION ${CMAKE_INSTALL_PREFIX}/include/Mango
)


target_include_directories(Widgets PUBLIC
    ${CMAKE_INSTALL_PREFIX}/include
)

install(TARGETS Widgets EXPORT WidgetsTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
    ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
    RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
    INCLUDES DESTINATION ${CMAKE_INSTALL_PREFIX}/include)

export(EXPORT WidgetsTargets FILE "${CMAKE_BINARY_DIR}/cmake/macos/WidgetsTargets.cmake" NAMESPACE Mango::)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    "${CMAKE_BINARY_DIR}/cmake/macos/WidgetsConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

configure_package_config_file(
    "${CMAKE_SOURCE_DIR}/cmake/macos/WidgetsConfig.cmake.in"
    "${CMAKE_BINARY_DIR}/cmake/macos/WidgetsConfig.cmake"
    PATH_VARS Widgets_INCLUDE_DIRS USER_HOME_DIR
    INSTALL_DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/cmake/Widgets
)

set(Mango_INCLUDE_DIRS "${CMAKE_INSTALL_PREFIX}/include")

install(FILES
    "${CMAKE_BINARY_DIR}/cmake/macos/WidgetsConfig.cmake"
    "${CMAKE_BINARY_DIR}/cmake/macos/WidgetsConfigVersion.cmake"
    "${CMAKE_BINARY_DIR}/cmake/macos/WidgetsTargets.cmake"
    DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/cmake/Widgets
)