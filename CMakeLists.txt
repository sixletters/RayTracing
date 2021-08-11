cmake_minimum_required(VERSION 3.7...3.18)

if(${CMAKE_VERSION} VERSION_LESS 3.12)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()

# Set up the project
project(Lab4 VERSION 1.0
             DESCRIPTION "CS3241 Lab Assignment 4"
             LANGUAGES CXX)

# Suppress generation of ZERO_CHECK build target
set(CMAKE_SUPPRESS_REGENERATION true)

# Add the executable
add_executable(${PROJECT_NAME} Main.cpp Camera.cpp Image.cpp ImageIO.cpp Raytrace.cpp Util.cpp Plane.cpp Sphere.cpp Triangle.cpp)

# Set the output directory to the top-level directory of the project
# without any Debug, Release, etc folders, so the freeglut.dll file can be read by the exe.
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/$<0:>)

# Include the stb_image directory
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include)
