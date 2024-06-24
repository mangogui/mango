cmake_minimum_required(VERSION 3.16)

project(MangoWidget VERSION 1.0.0)

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

add_subdirectory(plugins)

if(BUILD_ANDROID)
    message(STATUS "Configuring for Android")

    include_directories(${CMAKE_SOURCE_DIR}/plugins/platforms/android)

    add_subdirectory(android)

    # Set the path to the Android SDK
    set(ANDROID_SDK_ROOT ${USER_HOME_DIR}/Library/Android/sdk)
    set(ANDROID_ABI "arm64_v8a")

    # Define the source and destination directories
    set(ANDROID_DIR ${PROJECT_SOURCE_DIR}/android)
    set(DEST_DIR ${CMAKE_CURRENT_BINARY_DIR}/android)

    # Create the Mango executable
    add_library(Mango SHARED main.cpp plugins/platforms/android/Application.h
        plugins/platforms/android/Application.cpp)

    set(LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}/android/libs/${ANDROID_ABI}")

    # Include directories
    target_include_directories(Mango PRIVATE ${CMAKE_SOURCE_DIR}/plugins/platforms/android)

    target_include_directories(Mango PRIVATE ${CMAKE_SOURCE_DIR}/include)

    target_link_libraries(Mango PUBLIC MangoWidgets)

    # Add a custom target to build the Android project using Gradle
#    add_custom_target(build_android ALL
#        COMMAND ./gradlew build
#        WORKING_DIRECTORY ${DEST_DIR}
#        COMMENT "Building Android project using Gradle"
#    )
elseif (BUILD_MACOS)
    message(STATUS "Configuring for macOS")

    include_directories(${CMAKE_SOURCE_DIR}/plugins/platforms/macos)

    add_library(Mango main.cpp)

    target_include_directories(Mango PRIVATE ${CMAKE_SOURCE_DIR}/plugins/platforms/macos)

    target_link_libraries(Mango PRIVATE MangoWidgets)
endif ()


if(BUILD_ANDROID)
    # Set the install prefix for Android
    set(CMAKE_INSTALL_PREFIX "${USER_HOME_DIR}/Mango/0.0.0/android_${ANDROID_ABI}")

    target_include_directories(Mango PUBLIC
        ${CMAKE_INSTALL_PREFIX}/include
    )

    install(FILES
        plugins/platforms/android/Application.h
        # Add other header files here
        DESTINATION include/Mango
    )

    # Install the executable
    install(TARGETS Mango
        DESTINATION bin)

    set(Mango_INCLUDE_DIRS "${CMAKE_INSTALL_PREFIX}/include")

    install(TARGETS Mango MangoWidgets EXPORT MangoTargets
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
        INCLUDES DESTINATION include)

    export(EXPORT MangoTargets FILE "${CMAKE_CURRENT_BINARY_DIR}/cmake/android/MangoTargets.cmake" NAMESPACE Mango::)

    include(CMakePackageConfigHelpers)
    write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/cmake/android/MangoConfigVersion.cmake"
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY AnyNewerVersion
    )

    configure_package_config_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/cmake/android/MangoConfig.cmake.in"
        "${CMAKE_CURRENT_BINARY_DIR}/cmake/android/MangoConfig.cmake"
        PATH_VARS Mango_INCLUDE_DIRS USER_HOME_DIR
        INSTALL_DESTINATION lib/cmake/Mango
    )

    set(Mango_INCLUDE_DIRS "${CMAKE_INSTALL_PREFIX}/include")

    install(FILES
        "${CMAKE_CURRENT_BINARY_DIR}/cmake/android/MangoConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/cmake/android/MangoConfigVersion.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/cmake/android/MangoTargets.cmake"
        DESTINATION lib/cmake/Mango
    )

    set(ANDROID_DIR ${PROJECT_SOURCE_DIR}/android)
    set(INSTALL_DIR "${USER_HOME_DIR}/Mango/0.0.0/android_${ANDROID_ABI}")
    install(DIRECTORY ${ANDROID_DIR} DESTINATION ${INSTALL_DIR})

elseif (BUILD_MACOS)
    set(CMAKE_INSTALL_PREFIX "${USER_HOME_DIR}/Mango/0.0.0/macos")

    target_include_directories(Mango PUBLIC
        ${CMAKE_INSTALL_PREFIX}/include
    )

    set(Mango_INCLUDE_DIRS "${CMAKE_INSTALL_PREFIX}/include")

    install(TARGETS Mango MangoWidgets EXPORT MangoTargets
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
        INCLUDES DESTINATION include)

    export(EXPORT MangoTargets FILE "${CMAKE_CURRENT_BINARY_DIR}/cmake/macos/MangoTargets.cmake" NAMESPACE Mango::)

    include(CMakePackageConfigHelpers)
    write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/cmake/macos/MangoConfigVersion.cmake"
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY AnyNewerVersion
    )

    configure_package_config_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/cmake/macos/MangoConfig.cmake.in"
        "${CMAKE_CURRENT_BINARY_DIR}/cmake/macos/MangoConfig.cmake"
        PATH_VARS Mango_INCLUDE_DIRS USER_HOME_DIR
        INSTALL_DESTINATION lib/cmake/Mango
    )

    set(Mango_INCLUDE_DIRS "${CMAKE_INSTALL_PREFIX}/include")

    install(FILES
        "${CMAKE_CURRENT_BINARY_DIR}/cmake/macos/MangoConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/cmake/macos/MangoConfigVersion.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/cmake/macos/MangoTargets.cmake"
        DESTINATION lib/cmake/Mango
    )

endif()

