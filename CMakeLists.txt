#----------------------------------------
# Project: PicoAna
#----------------------------------------

# Minimum version of CMake
cmake_minimum_required(VERSION 3.16)

# Name and version of this project
project(PicoAna VERSION 1.0.0 LANGUAGES C CXX)

# Type of CMake
set(CMAKE_BUILD_TYPE Debug)

# External packages
find_package(ROOT REQUIRED COMPONENTS Spectrum)

# Set runtime output directory as bin
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin/)

# Add the directories
include_directories(./include/)
include_directories(./src/)
include(${ROOT_USE_FILE})

# Add executables
add_executable(iAna ana.cpp src/Convert.cpp
                            src/GlobalFit.cpp)

# Link libraries
target_link_libraries(iAna ${ROOT_LIBRARIES})

# Add commands to set up the environment, using setup.sh
execute_process(COMMAND cp ${CMAKE_CURRENT_SOURCE_DIR}/config/setup.sh ${PROJECT_BINARY_DIR})
execute_process(COMMAND sed -i "s:<PROJECT>:${CMAKE_CURRENT_SOURCE_DIR}:g" ${PROJECT_BINARY_DIR}/setup.sh)
execute_process(COMMAND chmod 775 ${PROJECT_BINARY_DIR}/setup.sh)