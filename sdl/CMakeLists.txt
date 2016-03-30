project (libCppGUI-SDL)

# Main target

add_library( ${PROJECT_NAME} STATIC

    src/dummy.cpp
    
    include/cppgui/sdl/Application.hpp include/cppgui/sdl/Application.ipp
    include/cppgui/sdl/Window.hpp include/cppgui/sdl/Window.ipp

    include/cppgui/sdl/Default_configuration.hpp # TODO: rename to Default_widget_configuration ?
    include/cppgui/sdl/Keyboard_adapter.hpp
    include/cppgui/sdl/Mouse_adapter.hpp
    include/cppgui/sdl/OpenGL_adapter.hpp

)

set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)

if (NOT TARGET libCppGUI)
    message(FATAL_ERROR "libCppGUI (main library) not defined as a CMake target")
endif()
target_link_libraries(${PROJECT_NAME} PUBLIC libCppGUI)

if (NOT TARGET libGPCGLWrappers)
    message(FATAL_ERROR "libGPCGLWrappers not defined as a CMake target")
endif()
target_link_libraries(${PROJECT_NAME} PUBLIC libGPCGLWrappers)

target_include_directories(${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)
