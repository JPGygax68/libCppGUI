project (libCppGUI)

#------------------------------------------------------------------------------
#  libCppGUI - A GUI library for C++11/14
#  
#  Copyright 2016,2017 Hans-Peter Gygax
#
#  Licensed under the Apache License, Version 2.0 (the "License");
#  you may not use this file except in compliance with the License.
#  You may obtain a copy of the License at
#
#  http://www.apache.org/licenses/LICENSE-2.0
#
#  Unless required by applicable law or agreed to in writing, software
#  distributed under the License is distributed on an "AS IS" BASIS,
#  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#  See the License for the specific language governing permissions and
#  limitations under the License.
#-----------------------------------------------------------------------------

# The core must be accessible to all components

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/include)

# TODO: also make all components accessible to each other ? AVOID IF POSSIBLE
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/builtin_drivers/renderer_opengl/include)
#include_directories(${CMAKE_CURRENT_BINARY_DIR}/builtin_drivers/renderer_opengl)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/builtin_drivers/platform_sdl2/include)
#include_directories(${CMAKE_CURRENT_BINARY_DIR}/builtin_drivers/platform_sdl2)


# Define main target: static library
add_library( ${PROJECT_NAME} 
STATIC
    include/cppgui/basic_types.hpp
    include/cppgui/unicode.hpp
    include/cppgui/Window.hpp           src/Window.cpp
    include/cppgui/Application.hpp      src/Application.cpp
    include/cppgui/Resource.hpp
    include/cppgui/Box.hpp              src/Box.cpp
    include/cppgui/Canvas.hpp           src/Canvas.cpp
    include/cppgui/Widget.hpp           src/Widget.cpp
    include/cppgui/Container.hpp        src/Container.cpp
    include/cppgui/Root_widget.hpp      src/Root_widget.cpp
    include/cppgui/Label.hpp            src/Label.cpp
    include/cppgui/Textbox.hpp          src/Textbox.cpp
)

# This will allow source files to include their contract header files without using paths
target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include/cppgui")

# Generate configuration file and make it available to target and consumers
configure_file("${PROJECT_SOURCE_DIR}/cppgui_config.in.hpp"
               "${PROJECT_BINARY_DIR}/cppgui_config.hpp" )
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_BINARY_DIR})

# Make public header files available to consumers
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

#-----------------------------------------------------------------------------
# Sub-libraries (= library components)
#

# Component sources must be able to access certain header files of libCppGUI core
include_directories(${PROJECT_NAME} "${CMAKE_CURRENT_SOURCE_DIR}/include")
# Component sources must also be allowed to access the generated config header file
include_directories(${PROJECT_NAME} "${CMAKE_CURRENT_BINARY_DIR}")

# "Baked" fonts

add_subdirectory(baked_fonts)
target_link_libraries(${PROJECT_NAME} PUBLIC cppgui_baked_fonts)

# Driver: Renderer
# TODO: make configurable
add_subdirectory(builtin_drivers/renderer_opengl)
target_link_libraries(${PROJECT_NAME} PUBLIC cppgui_renderer_opengl)

# Driver: Session manager (windows and input)
# TODO: make configurable
add_subdirectory(builtin_drivers/platform_sdl2)
target_link_libraries(${PROJECT_NAME} PUBLIC cppgui_platform_sdl2)


