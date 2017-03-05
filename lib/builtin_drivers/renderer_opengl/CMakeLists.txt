project(cppgui_renderer_opengl)

add_library(${PROJECT_NAME} STATIC 
    src/OpenGL_renderer.cpp include/cppgui/builtin_drivers/OpenGL_renderer.hpp
    include/cppgui/builtin_drivers/gl.h
    src/gl_shaderprogram.hpp src/gl_shaderprogram.cpp
)
    
# Source files need easy access to our gl.h OpenGL importer convenience
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/cppgui/builtin_drivers/)

# Note: this library is intended to be used as a CMake target only
target_include_directories(${PROJECT_NAME} BEFORE PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Generate configuration file and make it available to target and consumers
configure_file("${PROJECT_SOURCE_DIR}/cppgui_opengl_config.in.hpp"
               "${PROJECT_BINARY_DIR}/cppgui_opengl_config.hpp" )
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_BINARY_DIR})

# GLSL shader files

source_group("Shader files" FILES ${SHADER_FILES})  # for Visual Studio

# TODO: make this into a CMake module

set(GENERATED_HEADERS_DIR "${CMAKE_CURRENT_BINARY_DIR}/generated_headers")
target_include_directories(${PROJECT_NAME} PRIVATE ${GENERATED_HEADERS_DIR})

# Find the GPC Bin2C utility
if (NOT TARGET gpcbin2c)
    find_package(gpcbin2c REQUIRED)
endif()

# Add a custom target and a dependency for each shader file

foreach(shader "src/vertex.glsl" "src/fragment.glsl")
  get_filename_component(name "${shader}" NAME)
  set(shader_header "${GENERATED_HEADERS_DIR}/${name}.h")
  add_custom_command(
    OUTPUT ${shader_header}
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${shader}
    COMMAND gpcbin2c --input=${CMAKE_CURRENT_SOURCE_DIR}/${shader} --output=${shader_header}
  )
  target_sources(${PROJECT_NAME} PUBLIC ${shader_header})
endforeach()

# External dependencies

if (0)
if (NOT TARGET libGPCFonts)
    message(FATAL_ERROR "libGPCFonts not defined as a target")
endif()
target_link_libraries(${PROJECT_NAME} PUBLIC libGPCFonts)
endif(0)




