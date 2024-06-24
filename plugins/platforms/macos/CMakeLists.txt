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

add_library(MangoWidgets SHARED
    ${SOURCES})

target_link_libraries(MangoWidgets "-framework Cocoa" "-framework Metal" "-framework QuartzCore"
    "-framework MetalKit")

set(LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}/macos/libs")

