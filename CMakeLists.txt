cmake_minimum_required(VERSION 3.10)

# Project name and language
project(VkewWin32Project LANGUAGES C CXX)


# Add source files
set(SOURCES src/vkTest.cpp src/vkew.c)

# Add Vulkan include directory (assuming "vulkan/" is at the root)
include_directories(${CMAKE_SOURCE_DIR})

# Create the executable
add_executable(VkTestApp ${SOURCES})

# Cross-platform support (use Win32 subsystem if on Windows)
if(WIN32)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS")
    target_link_libraries(VkTestApp user32)  # Example: Link user32.lib for a Win32 app
endif()
