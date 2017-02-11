project(cppgui_renderer_opengl)

add_library(${PROJECT_NAME} STATIC 
    src/renderer_opengl.cpp 
    src/gl.h
    src/gl_shaderprogram.hpp src/gl_shaderprogram.cpp
)
    
# Note: this library is intended to be used as a CMake target only
target_include_directories(${PROJECT_NAME} BEFORE PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

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

if (NOT TARGET libGPCFonts)
    message(FATAL_ERROR "libGPCFonts not defined as a target")
endif()
target_link_libraries(${PROJECT_NAME} PUBLIC libGPCFonts)




