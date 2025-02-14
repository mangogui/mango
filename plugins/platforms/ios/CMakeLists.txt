cmake_minimum_required(VERSION 3.16)

set(SOURCES
    Widget.mm
    Application.mm
    SceneDelegate.mm
    ViewController.mm
)

set(HEADERS
    Widget.h
    Widgets.h
    Application.h
)

add_library(Widgets SHARED
    ${HEADERS}
    ${SOURCES})

find_library(UIKIT UIKit)
find_library(FOUNDATION Foundation)

message("Check if all required frameworks are found")

target_link_libraries(Widgets
    ${UIKIT}
    ${FOUNDATION}
)

set(LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}/ios/libs")
set(Widgets_INCLUDE_DIRS "${CMAKE_INSTALL_PREFIX}/include")

install(FILES
    ${HEADERS}
    DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/Widgets.framework/Headers
)

target_include_directories(Widgets PUBLIC
    ${CMAKE_INSTALL_PREFIX}/include
)

if (CMAKE_SYSTEM_NAME STREQUAL "iOS")
    # Set the development team ID
    set(DEVELOPMENT_TEAM_ID "GU2Z9QLD7V")

    # Set the target properties for the Widgets library
    set_target_properties(Widgets PROPERTIES
        FRAMEWORK TRUE
        MACOSX_FRAMEWORK_IDENTIFIER com.mango.widgets
        XCODE_ATTRIBUTE_DEVELOPMENT_TEAM ${DEVELOPMENT_TEAM_ID}
        XCODE_ATTRIBUTE_CODE_SIGN_STYLE "Automatic"
        XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "Apple Development: parhamoyan@yahoo.com (GU2Z9QLD7V)"
        XCODE_ATTRIBUTE_CODE_SIGNING_REQUIRED "YES"
        XCODE_ATTRIBUTE_IPHONEOS_DEPLOYMENT_TARGET "14.0"
        XCODE_ATTRIBUTE_SDKROOT "iphonesimulator"
        PUBLIC_HEADER Widgets.h
    )

    message("Foundation framework found at ${FOUNDATION_LIBRARY}")
    message("Metal framework found at ${METAL_LIBRARY}")
    message("QuartzCore framework found at ${QUARTZCORE_LIBRARY}")
    message("MetalKit framework found at ${METALKIT_LIBRARY}")
    message("UIKit framework found at ${UIKIT_LIBRARY}")
endif ()

install(TARGETS Widgets EXPORT WidgetsTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
    ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
    RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
    INCLUDES DESTINATION ${CMAKE_INSTALL_PREFIX}/include
    FRAMEWORK DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_PREFIX}/include/Mango
)

export(EXPORT WidgetsTargets FILE "${CMAKE_BINARY_DIR}/cmake/ios/WidgetsTargets.cmake" NAMESPACE Mango::)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    "${CMAKE_BINARY_DIR}/cmake/ios/WidgetsConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

configure_package_config_file(
    "${CMAKE_SOURCE_DIR}/cmake/ios/WidgetsConfig.cmake.in"
    "${CMAKE_BINARY_DIR}/cmake/ios/WidgetsConfig.cmake"
    PATH_VARS Widgets_INCLUDE_DIRS USER_HOME_DIR
    INSTALL_DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/cmake/Widgets
)

set(Mango_INCLUDE_DIRS "${CMAKE_INSTALL_PREFIX}/include")

install(FILES
    "${CMAKE_BINARY_DIR}/cmake/ios/WidgetsConfig.cmake"
    "${CMAKE_BINARY_DIR}/cmake/ios/WidgetsConfigVersion.cmake"
    "${CMAKE_BINARY_DIR}/cmake/ios/WidgetsTargets.cmake"
    DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/cmake/Widgets
)