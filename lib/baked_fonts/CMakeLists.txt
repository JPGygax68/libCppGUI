# Sub-library for libCppGUI, packaging pre-rasterized fonts

project(cppgui_baked_fonts)

# TODO: this might actually belong with the font rasterizer project
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
    CODEPOINTS $39 $25 $3C $3E $5B $5D
)

# TODO: auto-generate from list of sizes
file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/generated/fonts/all_baked_icon_fonts.h 
    "#include <modernpics-18.h>\n"
    "#include <modernpics-24.h>\n"
)
target_include_directories(${PROJECT_NAME} PUBLIC  ${CMAKE_CURRENT_BINARY_DIR}/generated/fonts)

target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/include/cppgui)
target_include_directories(${PROJECT_NAME} PUBLIC  ${PROJECT_SOURCE_DIR}/include)

# Generate configuration file and make it available to target and consumers
configure_file("${PROJECT_SOURCE_DIR}/cppgui_baked_fonts_config.in.hpp"
               "${PROJECT_BINARY_DIR}/cppgui_baked_fonts_config.hpp" )
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_BINARY_DIR})


