#------------------------------------------------------------------------------
#  libCppGUI - A GUI library for C++11/14
#  
#  Copyright 2016 Hans-Peter Gygax
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

#=============================================================================
# Skeleton for a libCppGUI-SDL application.
#=============================================================================

# TODO: select a name for your application
project (my_cppgui_app)

# The main target

add_executable( ${PROJECT_NAME}
  
  main.cpp

  Main_window.hpp           Main_window.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/generated/headers)

target_link_libraries( ${PROJECT_NAME} 
PRIVATE 
    SDL2-static SDL2main glbinding libGPCGLWrappers libGPCGUIRenderer libGPCGUIGLRenderer libCppGUI libCppGUI-SDL
)
target_include_directories( ${PROJECT_NAME} PRIVATE SDL2-static)