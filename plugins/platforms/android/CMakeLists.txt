# Define the source and destination directories
set(ANDROID_DIR ${PROJECT_SOURCE_DIR}/android)
set(DEST_DIR ${CMAKE_CURRENT_BINARY_DIR}/android)

# Define the list of source files
set(SOURCES
    JniObject.h
    JniObject.cpp
    JniEnvironment.h
    Widget.h
    Widget.cpp
    Application.h
    Application.cpp
    my-lib.cpp
)

# Create the shared library
add_library(Widgets SHARED
    ${SOURCES})

# Link necessary libraries
target_link_libraries(Widgets
    android
    EGL
    log)

set(Widgets_INCLUDE_DIRS "${CMAKE_INSTALL_PREFIX}/include")

install(FILES
    JniObject.h
    Widget.h
    JniEnvironment.h
    Application.h
    DESTINATION ${CMAKE_INSTALL_PREFIX}/include/Mango
)

target_include_directories(Widgets PUBLIC
    ${CMAKE_INSTALL_PREFIX}/include
)

install(TARGETS Widgets EXPORT MangoTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
    ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
    RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
    INCLUDES DESTINATION ${CMAKE_INSTALL_PREFIX}/include)

export(EXPORT MangoTargets FILE "${CMAKE_BINARY_DIR}/cmake/android/WidgetsTargets.cmake" NAMESPACE Mango::)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    "${CMAKE_BINARY_DIR}/cmake/android/WidgetsConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

configure_package_config_file(
    "${CMAKE_SOURCE_DIR}/cmake/android/WidgetsConfig.cmake.in"
    "${CMAKE_BINARY_DIR}/cmake/android/WidgetsConfig.cmake"
    PATH_VARS Widgets_INCLUDE_DIRS USER_HOME_DIR
    INSTALL_DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/cmake/Widgets
)

install(FILES
    "${CMAKE_BINARY_DIR}/cmake/android/WidgetsConfig.cmake"
    "${CMAKE_BINARY_DIR}/cmake/android/WidgetsConfigVersion.cmake"
    "${CMAKE_BINARY_DIR}/cmake/android/WidgetsTargets.cmake"
    DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/cmake/Widgets
)

set(ANDROID_DIR ${PROJECT_SOURCE_DIR}/android)
set(INSTALL_DIR "${USER_HOME_DIR}/Mango/0.0.0/android_${ANDROID_ABI_FOLDER_NAME}")
install(DIRECTORY ${ANDROID_DIR} DESTINATION ${INSTALL_DIR})
