cmake_minimum_required(VERSION 3.27)
project(MangoBasedProject)

set(CMAKE_CXX_STANDARD 17)


set(Widgets_DIR "/Users/parhamoyan/Mango/0.0.0/macos/lib/cmake/Widgets")


find_package(Widgets REQUIRED)

add_executable(MangoBasedProject main.cpp Switch.h Switch.cpp)

target_include_directories(MangoBasedProject PUBLIC "/Users/parhamoyan/Mango/0.0.0/macos/include")

target_link_libraries(MangoBasedProject PUBLIC Mango::Widgets)

