cmake_minimum_required(VERSION 3.20)

project(vkew LANGUAGES C CXX)

# ------------------------------------------------------------
# Options
# ------------------------------------------------------------
option(VKEW_ENABLE_TESTS "Enable vkew tests" OFF)

# ------------------------------------------------------------
# Common configuration
# ------------------------------------------------------------
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Allow consumers to use: target_link_libraries(app PRIVATE vkew::vkew)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# ------------------------------------------------------------
# vkew library (static)
# ------------------------------------------------------------
add_library(vkew STATIC)
add_library(vkew::vkew ALIAS vkew)

target_sources(vkew PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/vkew.c
)

target_include_directories(vkew PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_definitions(vkew PRIVATE
    VK_NO_PROTOTYPES=1
)

if (WIN32)
    target_compile_definitions(vkew PRIVATE IS_PLATFORM_WIN=1)
endif()

if (MSVC)
    target_compile_options(vkew PRIVATE /W4)
else()
    target_compile_options(vkew PRIVATE -Wall -Wextra)
endif()

set_target_properties(vkew PROPERTIES
    FOLDER "vkew"
)

# Try to find Vulkan SDK
find_package(Vulkan QUIET)
if (Vulkan_FOUND)
    target_link_libraries(vkew PUBLIC Vulkan::Vulkan)
    target_compile_definitions(vkew PUBLIC VKEW_VULKAN_SDK=1)
else()
    message(STATUS "Vulkan not found via find_package(Vulkan). Library will compile, but link may fail unless the application links Vulkan loader.")
endif()

# ------------------------------------------------------------
# Tests
# ------------------------------------------------------------
if(VKEW_ENABLE_TESTS)
    enable_testing()

    if (WIN32)
        add_executable(vkew_test WIN32)
    else()
        add_executable(vkew_test)
    endif()

    set_target_properties(vkew_test PROPERTIES
        FOLDER "vkew_tests"
    )

    # Test sources
    target_sources(vkew_test PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/tests/vkTest.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/tests/vkSwapChain.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/tests/vkTestGraphics.cpp
    )

    target_include_directories(vkew_test PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/tests
    )

    # Link vkew library
    target_link_libraries(vkew_test PRIVATE vkew::vkew)

    if (WIN32)
        target_compile_definitions(vkew_test PRIVATE IS_PLATFORM_WIN=1)
        target_link_libraries(vkew_test PRIVATE user32)
    endif()

    # Link Vulkan if found
    if (Vulkan_FOUND)
        target_link_libraries(vkew_test PRIVATE Vulkan::Vulkan)
    endif()
     

    # Optional: Add SPIR-V compilation if glslc is available
    find_program(GLSLC_EXECUTABLE glslc)
    
    if(GLSLC_EXECUTABLE)
        set(SPIRV_OUTPUT_DIR "$<TARGET_FILE_DIR:vkew_test>/shaders/spirv")
        set(SPIRV_STAMP_FILE "${CMAKE_CURRENT_BINARY_DIR}/vkew_spirv_compiled.stamp")
        
        add_custom_command(
            OUTPUT ${SPIRV_STAMP_FILE}
            COMMAND ${CMAKE_COMMAND} -E make_directory "${SPIRV_OUTPUT_DIR}"
            COMMAND ${GLSLC_EXECUTABLE}
                "${CMAKE_CURRENT_SOURCE_DIR}/tests/shaders/triangle_vs.vert"
                -o "${SPIRV_OUTPUT_DIR}/test_vs.spv"
            COMMAND ${GLSLC_EXECUTABLE}
                "${CMAKE_CURRENT_SOURCE_DIR}/tests/shaders/triangle_ps.frag"
                -o "${SPIRV_OUTPUT_DIR}/test_ps.spv"
            COMMAND ${CMAKE_COMMAND} -E touch ${SPIRV_STAMP_FILE}
            DEPENDS 
                ${CMAKE_CURRENT_SOURCE_DIR}/tests/shaders/test_vs.vert
                ${CMAKE_CURRENT_SOURCE_DIR}/tests/shaders/test_ps.frag
            COMMENT "Compiling GLSL shaders to SPIR-V in ${SPIRV_OUTPUT_DIR}"
            VERBATIM
        )
        
        add_custom_target(compile_vkew_spirv_shaders 
            DEPENDS ${SPIRV_STAMP_FILE}
        )
        
        add_dependencies(vkew_test compile_vkew_spirv_shaders)
    endif()

endif()
