project (test_app)

# Rasterized fonts using the GPCFontRasterizer and the GPCBin2C utilities

if (NOT TARGET gpcfontrasterizer)
    message(FATAL_ERROR "gpcfontrasterizer must be available as a CMake target")
endif()
if (NOT TARGET gpcbin2c)
    message(FATAL_ERROR "gpcbin2c must be available as a CMake target")
endif()

if (0)
set(font_file LiberationSans-Regular)

foreach(size IN ITEMS 
16 24)

    add_custom_command(
        OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/generated/rastfont/${font_file}-${size}.rastfont
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/data/fonts/${font_file}.ttf
        COMMAND gpcfontrasterizer ARGS
            input=${CMAKE_CURRENT_SOURCE_DIR}/data/fonts/${font_file}.ttf
            size=${size}
            output="${CMAKE_CURRENT_BINARY_DIR}/generated/rastfont/${font_file}-${size}.rastfont"
            range=all
    )

    add_custom_command(
        OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/generated/headers/${font_file}-${size}.h
        DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/generated/rastfont/${font_file}-${size}.rastfont
        COMMAND GPCBin2C ARGS
            -i "${CMAKE_CURRENT_BINARY_DIR}/generated/rastfont/${font_file}-${size}.rastfont"
            -o "${CMAKE_CURRENT_BINARY_DIR}/generated/headers/${font_file}-${size}.h"
    )

    list(APPEND FONT_HEADERS ${CMAKE_CURRENT_BINARY_DIR}/generated/headers/${font_file}-${size}.h)

endforeach(size)
endif()

# The main target

add_executable( ${PROJECT_NAME}
  
  main.cpp

  Test_window.hpp       Test_window.cpp
  #Application.hpp       Application.ipp
  #Window.hpp            Window.ipp

  # ${FONT_HEADERS}
)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/generated/headers)

target_link_libraries( ${PROJECT_NAME} 
PRIVATE 
    SDL2-static SDL2main GLBinding libGPCGLWrappers libGPCGUIRenderer libGPCGUIGLRenderer libCppGUI libCppGUI-SDL
)
target_include_directories( ${PROJECT_NAME} PRIVATE SDL2-static)