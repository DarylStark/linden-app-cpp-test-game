cmake_minimum_required(VERSION 3.20)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set variables
set(PROJECT_NAME linden-app-cpp-test-game)
set(PROJECT_VERSION 0.0.1)
set(EXECUTABLE_NAME ${PROJECT_NAME})

# Project name
project(${PROJECT_NAME} VERSION ${PROJECT_VERSION})

# Set the C++ files to compile
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Add dependencies
add_subdirectory(dependencies/linden-lib-cpp-framework)

# Add includepath
include_directories(dependencies/linden-lib-cpp-framework/src)

# Define the executable
add_executable(${EXECUTABLE_NAME} ${SOURCES})

# Add dependencies
target_link_libraries(${EXECUTABLE_NAME} linden-lib-cpp-framework)
