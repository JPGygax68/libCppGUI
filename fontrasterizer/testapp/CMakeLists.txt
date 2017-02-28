cmake_minimum_required(VERSION 3.0)

#------------------------------------------------
# Project + basics
#

project(GPCFontRasterizer_TestApp)

#------------------------------------------------
# Configuration
#

#set(LOCSIM_DEV_ROOT "D:/Projects/Locsim" CACHE PATH "Root of Locsim development directory")
	
#------------------------------------------------
# Special settings
#

# Force static runtime libraries
if(0)
FOREACH(flag
  CMAKE_C_FLAGS_RELEASE CMAKE_C_FLAGS_RELWITHDEBINFO
  CMAKE_C_FLAGS_DEBUG CMAKE_C_FLAGS_DEBUG_INIT
  CMAKE_CXX_FLAGS_RELEASE  CMAKE_CXX_FLAGS_RELWITHDEBINFO
  CMAKE_CXX_FLAGS_DEBUG  CMAKE_CXX_FLAGS_DEBUG_INIT)
  STRING(REPLACE "/MD" "/MT" "${flag}" "${${flag}}")
  SET("${flag}" "${${flag}} /EHsc")
ENDFOREACH()
endif(0)
 
include_directories(${CMAKE_CURRENT_BINARY_DIR}/generated)

set(TEMP_DIR "${CMAKE_CURRENT_BINARY_DIR}/tmp")

#------------------------------------------------
# Targets
#

# Embedded rasterized font

# 1) Generate a rasterized font file

set(RASTERIZED_FONT_FILE_NAME "arial-14.15.bin")
set(RASTERIZED_FONT_FILE_PATH "${TEMP_DIR}/${RASTERIZED_FONT_FILE_NAME}")

add_custom_command( OUTPUT "${RASTERIZED_FONT_FILE_PATH}"
	COMMAND GPCFontRasterizer ARGS input=Arial.ttf size=14 size=15 output=${RASTERIZED_FONT_FILE_PATH}
)

# 2) Embed it

set(RASTERIZED_FONT_INCLUDE_FILE_NAME "embedded_font.h")
set(RASTERIZED_FONT_INCLUDE_FILE_PATH "${CMAKE_CURRENT_BINARY_DIR}/generated/${RASTERIZED_FONT_INCLUDE_FILE_NAME}")

if (NOT TARGET gpcbin2c)
    find_package(gpcbin2c REQUIRED)
endif()

add_custom_command( OUTPUT "${RASTERIZED_FONT_INCLUDE_FILE_PATH}"
	DEPENDS ${RASTERIZED_FONT_FILE_PATH}
	COMMAND gpcbin2c ARGS -i "${RASTERIZED_FONT_FILE_PATH}" -o "${RASTERIZED_FONT_INCLUDE_FILE_PATH}"
)

# Embedded shaders

set(VERTEX_SHADER_SOURCE_FILE "TextRenderer.vertex.glsl")
set(VERTEX_SHADER_INCLUDE_FILE_NAME "TextRenderer_vertex_shader.h")

set(FRAGMENT_SHADER_SOURCE_FILE "TextRenderer.fragment.glsl")
set(FRAGMENT_SHADER_INCLUDE_FILE_NAME "TextRenderer_fragment_shader.h")

set(SHADER_SOURCE_FILES ${VERTEX_SHADER_SOURCE_FILE} ${FRAGMENT_SHADER_SOURCE_FILE})

set(SHADER_INCLUDE_FILES "")

foreach(f VERTEX FRAGMENT) 
  set(${f}_SHADER_INCLUDE_FILE_PATH "${CMAKE_CURRENT_BINARY_DIR}/generated/${${f}_SHADER_INCLUDE_FILE_NAME}")
  add_custom_command( OUTPUT "${${f}_SHADER_INCLUDE_FILE_PATH}"
    DEPENDS ${${f}_SHADER_SOURCE_FILE}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMAND GPCBin2C ARGS -i "${${f}_SHADER_SOURCE_FILE}" -o "${${f}_SHADER_INCLUDE_FILE_PATH}"
  )
  set(SHADER_INCLUDE_FILES ${SHADER_INCLUDE_FILES} "${${f}_SHADER_INCLUDE_FILE_PATH}")
endforeach()

add_executable(${PROJECT_NAME} WIN32 main.cpp TextRenderer.cpp TextRenderer.hpp OpenGL/error.hpp 
  ${RASTERIZED_FONT_INCLUDE_FILE_PATH} ${SHADER_SOURCE_FILES} ${SHADER_INCLUDE_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

#------------------------------------------------
# Libraries we need
#

# GPC Fonts

find_package(libGPCFonts REQUIRED NO_CMAKE_ENVIRONMENT_PATH)
target_link_libraries(${PROJECT_NAME} libGPCFonts)

# SDL2

find_package(SDL2 REQUIRED)
target_include_directories(${PROJECT_NAME} PRIVATE ${SDL2_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARY} ${SDL2MAIN_LIBRARY})

# GLEW

find_package(GLEW REQUIRED)
target_include_directories(${PROJECT_NAME} PRIVATE ${GLEW_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} ${GLEW_LIBRARY})

# OpenGL 

find_package(OpenGL)
target_link_libraries(${PROJECT_NAME} ${OPENGL_gl_LIBRARY} ${OPENGL_glu_LIBRARY})
