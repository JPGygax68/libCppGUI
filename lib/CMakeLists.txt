project (libCppGUI)

# Rasterized fonts (for glyphs) using the GPCFontRasterizer and the GPCBin2C utilities

if (NOT TARGET gpcfontrasterizer)
    message(FATAL_ERROR "gpcfontrasterizer must be available as a CMake target")
endif()
if (NOT TARGET gpcbin2c)
    message(FATAL_ERROR "gpcbin2c must be available as a CMake target")
endif()

set(font_file LiberationSans-Regular)

foreach(size IN ITEMS 24)

    add_custom_command(
        OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/generated/rastfont/${font_file}-${size}.rastfont
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/data/fonts/${font_file}.ttf
        COMMAND gpcfontrasterizer ARGS
            input=${CMAKE_CURRENT_SOURCE_DIR}/data/fonts/${font_file}.ttf
            size=${size}
            output="${CMAKE_CURRENT_BINARY_DIR}/generated/rastfont/${font_file}-${size}.rastfont"
            range=geometric-shapes
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

# Main target

add_library( ${PROJECT_NAME} STATIC

    src/dummy.cpp

    include/cppgui/basic_types.hpp
    include/cppgui/Canvas.hpp                       include/cppgui/Canvas.ipp
    include/cppgui/Widget.hpp                       include/cppgui/Widget.ipp
    include/cppgui/Button.hpp                       include/cppgui/Button.ipp
    include/cppgui/Label.hpp                        include/cppgui/Label.ipp
    include/cppgui/Textbox.hpp                      include/cppgui/Textbox.ipp
    include/cppgui/Box.hpp                          include/cppgui/Box.ipp
    include/cppgui/Container.hpp                    include/cppgui/Container.ipp
    include/cppgui/Stack.hpp                        include/cppgui/Stack.ipp
    include/cppgui/Root_widget.hpp                  include/cppgui/Root_widget.ipp
    include/cppgui/Resource_mapper.hpp              include/cppgui/Resource_mapper.ipp
    include/cppgui/Cheap_resource_mapper.hpp        include/cppgui/Cheap_resource_mapper.ipp
    include/cppgui/Full_resource_mapper.hpp         include/cppgui/Full_resource_mapper.ipp
    include/cppgui/layouting.hpp                    include/cppgui/layouting.ipp
    #include/cppgui/Stylesheet.hpp
    include/cppgui/aspects.hpp

    include/cppgui/GUI_window.hpp include/cppgui/GUI_window.ipp

    include/cppgui/all_widgets.hpp
    include/cppgui/all_widgets.ipp

    ${FONT_HEADERS}
)

if (NOT TARGET libGPCFonts)
    message(FATAL_ERROR "libGPCFonts not defined as a CMake target")
endif()
target_link_libraries(${PROJECT_NAME} PRIVATE libGPCFonts)

target_include_directories(${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)