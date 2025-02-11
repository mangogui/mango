cmake_minimum_required(VERSION 3.16)

project(Mango VERSION 0.0.0 LANGUAGES C CXX OBJC OBJCXX)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_ANDROID "Build for Android" OFF)
option(BUILD_IOS "Build for iOS" OFF)
option(BUILD_MACOS "Build for macOS" OFF)
option(BUILD_WINDOWS "Build for Windows" OFF)

if(UNIX)
    set(USER_HOME_DIR $ENV{HOME})
elseif(WIN32)
    set(USER_HOME_DIR $ENV{USERPROFILE})
endif()

if(BUILD_ANDROID)
    # Set the path to the Android SDK
    set(ANDROID_ABI_FOLDER_NAME "arm64_v8a")
    set(CMAKE_INSTALL_PREFIX "${USER_HOME_DIR}/Mango/0.0.0/android_${ANDROID_ABI_FOLDER_NAME}")
elseif (BUILD_MACOS)
    set(CMAKE_INSTALL_PREFIX "${USER_HOME_DIR}/Mango/0.0.0/macos")
elseif (BUILD_IOS)
    set(CMAKE_INSTALL_PREFIX "${USER_HOME_DIR}/Mango/0.0.0/ios")
    set(CMAKE_OSX_ARCHITECTURES "arm64")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fobjc-arc -isysroot ${CMAKE_OSX_SYSROOT}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fobjc-arc -isysroot ${CMAKE_OSX_SYSROOT}")
endif ()

add_subdirectory(plugins)
#add_subdirectory(example)

if(BUILD_ANDROID)
    add_subdirectory(android)
endif ()




