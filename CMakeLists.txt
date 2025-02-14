cmake_minimum_required(VERSION 3.16)

# Define the project
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

# Export targets
install(
    EXPORT ${PROJECT_NAME}Targets
    FILE ${PROJECT_NAME}Targets.cmake
    NAMESPACE Mango::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)

# Define Mango installation directory
set(${PROJECT_NAME}_DIR "${CMAKE_INSTALL_PREFIX}/lib/cmake/${PROJECT_NAME}")

# Configure package version file
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    "${CMAKE_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

# Configure the config file
configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/${PROJECT_NAME}Config.cmake.in"
    "${CMAKE_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
    INSTALL_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
    PATH_VARS CMAKE_INSTALL_INCLUDEDIR
)

# Install config files
install(
    FILES
        "${CMAKE_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
        "${CMAKE_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
)

export(PACKAGE Mango)

message(STATUS "Mango installation directory: ${CMAKE_INSTALL_PREFIX}")
message(STATUS "Mango CMake configuration files installed to: ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}")