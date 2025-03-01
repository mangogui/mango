cmake_minimum_required(VERSION 3.16)
project(MangoBasedProject)

set(CMAKE_CXX_STANDARD 17)
find_package(Mango REQUIRED COMPONENTS Widgets Core)

add_executable(MangoBasedProject main.cpp Switch.h Switch.cpp)

target_link_libraries(MangoBasedProject PRIVATE Mango::Widgets Mango::Core)
