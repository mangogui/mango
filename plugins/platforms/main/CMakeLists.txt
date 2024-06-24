# Specify the CMake minimum version required
cmake_minimum_required(VERSION 3.22.1)

# Add the static library for main.cpp
add_library(main STATIC main.cpp)

# Optionally, specify include directories if main.cpp depends on headers in the parent project
target_include_directories(main PRIVATE ${CMAKE_SOURCE_DIR}/include)

# Optionally, specify any additional libraries that main.cpp might need
# target_link_libraries(main PRIVATE some_other_library)