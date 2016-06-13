project (CppGUI)

cmake_minimum_required(VERSION 3.4)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

set (CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMake")
include(cotire)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(ConanDeps.cmake)

#add_subdirectory(external/cereal)
#include_directories(external/cereal/include)
add_subdirectory(external/SDL2)
include_directories(external/SDL2/include)
add_subdirectory(external/glbinding)
add_subdirectory(external/libGPCGLWrappers/lib)
add_subdirectory(external/libGPCFonts)
add_subdirectory(external/GPCBin2C)
#add_subdirectory(external/GPCFontRasterizer/rasterizer)
add_subdirectory(external/libGPCGUIRenderer/lib)
add_subdirectory(external/libGPCGLUtils/lib)
add_subdirectory(external/libGPCGUIGLRenderer/lib)

add_subdirectory(lib)
add_subdirectory(sdl)

add_subdirectory(test_app)
