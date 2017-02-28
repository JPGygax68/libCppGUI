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

# Define main target: static library
add_library( ${PROJECT_NAME} 
STATIC
    include/cppgui/Widget.hpp           src/Widget.cpp
)

# This will allow source files to include their contract header files without using paths
target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include/cppgui")

# Generate configuration file and make it available to target and consumers
configure_file("${PROJECT_SOURCE_DIR}/cppgui_config.in.hpp"
               "${PROJECT_BINARY_DIR}/cppgui_config.hpp" )
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_BINARY_DIR})

# Make public header files available to consumers
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

#------------------------------------------------------------------------------
# "Drivers"
#

include_directories(${PROJECT_NAME} "${CMAKE_CURRENT_SOURCE_DIR}/include")

# Renderer
# TODO: make configurable
add_subdirectory(builtin_drivers/renderer_opengl)
target_link_libraries(${PROJECT_NAME} PUBLIC cppgui_renderer_opengl)

# Session manager (windows and input)
# TODO: make configurable
add_subdirectory(builtin_drivers/session_sdl2)
target_link_libraries(${PROJECT_NAME} PUBLIC cppgui_session_sdl2)


