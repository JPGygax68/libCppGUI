# Sub-library for libCppGUI, packaging pre-rasterized fonts

project(cppgui_baked_fonts)

include("${CMAKE_CURRENT_SOURCE_DIR}/BakeFontFile.cmake")


#------------------------------------------------------------------------------
# Prerequisite: font rasterizer command-line tool
#

add_subdirectory(rasterizer)


#------------------------------------------------------------------------------
# Main target: static sub-library
#

add_library(${PROJECT_NAME} STATIC
    include/cppgui/baked_fonts.hpp      src/baked_fonts.cpp
    ${PROJECT_BINARY_DIR}/cppgui_baked_fonts_config.hpp
)

target_bake_font(${PROJECT_NAME} "${CMAKE_CURRENT_SOURCE_DIR}/data/fonts/LiberationSans-Regular.ttf" 
    SIZES 16 24 
    ALL
)

target_bake_font(${PROJECT_NAME} "${CMAKE_CURRENT_SOURCE_DIR}/data/fonts/modernpics.otf"             
    SIZES 18 24 
    CODEPOINTS $25 $3E
)

target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/include/cppgui)
target_include_directories(${PROJECT_NAME} PUBLIC  ${PROJECT_SOURCE_DIR}/include)

# Generate configuration file and make it available to target and consumers
configure_file("${PROJECT_SOURCE_DIR}/cppgui_baked_fonts_config.in.hpp"
               "${PROJECT_BINARY_DIR}/cppgui_baked_fonts_config.hpp" )
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_BINARY_DIR})


