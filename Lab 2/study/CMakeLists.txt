cmake_minimum_required(VERSION 3.0.0)
project(study_lab3 VERSION 0.1.0)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE) # Don't build Documentation
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE) # Don't build Tests
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # Don't build Examples
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE) # Don't build Installation Information
set(GLFW_USE_HYBRID_HPG ON CACHE BOOL "" FORCE) # Add variables to use High Performance Graphics Card if available

add_subdirectory(vendor/glfw) # add glfw library

# to define the include directories (.h) 34an ma7tg4 aktb al path kolo we na ba3ml include
include_directories(
    vendor/glfw/include
    vendor/glad/include
)

# To change the output directory of the executable file
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin) # set output directory for executables
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/bin) # set output directory for executables
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/bin) # set output directory for executables

add_executable(study_lab3 main.cpp vendor/glad/src/gl.c)

# connect the executable file with the library
target_link_libraries(study_lab3 glfw)