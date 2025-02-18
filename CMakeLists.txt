cmake_minimum_required(VERSION 3.16)
project(Mango VERSION 0.0.0 LANGUAGES C CXX OBJC OBJCXX)


# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable installation paths
include(GNUInstallDirs)

# Define platform options
option(BUILD_ANDROID "Build for Android" OFF)
option(BUILD_IOS "Build for iOS" OFF)
option(BUILD_WINDOWS "Build for Windows" OFF)
option(BUILD_MACOS "Build for macOS" OFF)


# Detect platform and set user home directory
if(UNIX)
    set(USER_HOME_DIR $ENV{HOME})
elseif(WIN32)
    set(USER_HOME_DIR $ENV{USERPROFILE})
endif()

# Define installation prefix based on platform
if(BUILD_ANDROID)
    set(ANDROID_ABI_FOLDER_NAME "arm64_v8a")
    set(CMAKE_INSTALL_PREFIX "${USER_HOME_DIR}/${PROJECT_NAME}/${PROJECT_VERSION}/android_${ANDROID_ABI_FOLDER_NAME}")
elseif(BUILD_MACOS)
    set(CMAKE_INSTALL_PREFIX "${USER_HOME_DIR}/${PROJECT_NAME}/${PROJECT_VERSION}/macos")
elseif(BUILD_IOS)
    set(CMAKE_INSTALL_PREFIX "${USER_HOME_DIR}/${PROJECT_NAME}/${PROJECT_VERSION}/ios")
    set(CMAKE_OSX_ARCHITECTURES "arm64")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fobjc-arc -isysroot ${CMAKE_OSX_SYSROOT}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fobjc-arc -isysroot ${CMAKE_OSX_SYSROOT}")
endif()

set(MANGO_LIBRARY_DIR "${CMAKE_INSTALL_PREFIX}/lib")

add_subdirectory(core)

if(BUILD_ANDROID)
    add_subdirectory(plugins/platforms/android)
    add_subdirectory(android)
elseif(BUILD_IOS)
    add_subdirectory(plugins/platforms/ios)
elseif(BUILD_WINDOWS)
    add_subdirectory(plugins/platforms/windows)
elseif(BUILD_MACOS)
    add_subdirectory(plugins/platforms/macos)
endif()

# Install and export the package
install(EXPORT MangoTargets
    FILE MangoTargets.cmake
    NAMESPACE Mango::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Mango
)

include(CMakePackageConfigHelpers)

# Generate and install package configuration files
configure_package_config_file(
    cmake/MangoConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/MangoConfig.cmake
    INSTALL_DESTINATION lib/cmake/Mango
    PATH_VARS CMAKE_INSTALL_INCLUDEDIR CMAKE_INSTALL_PREFIX
)

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/MangoConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/MangoConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/MangoConfigVersion.cmake
    DESTINATION lib/cmake/Mango
)

export(PACKAGE Mango)