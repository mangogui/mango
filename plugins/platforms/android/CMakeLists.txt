# Define the list of source files
set(SOURCES
    JniObject.h
    JniObject.cpp
    JniEnvironment.h
    Widget.h
    Widget.cpp
    my-lib.cpp
)

# Create the shared library
add_library(MangoWidgets OBJECT
    ${SOURCES})

# Link necessary libraries
target_link_libraries(MangoWidgets
    android
    EGL
    log)

set(LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}/android/libs/${ANDROID_ABI}")


install(FILES
    JniObject.h
    Widget.h
    JniEnvironment.h
    DESTINATION include/Mango
)
