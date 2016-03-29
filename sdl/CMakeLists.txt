project (libCppGUI-SDL)

# Main target

add_library( ${PROJECT_NAME} STATIC

  include/cppgui/sdl/Application.hpp include/cppgui/sdl/Application.ipp
  include/cppgui/sdl/Window.hpp include/cppgui/sdl/Window.ipp
)

set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)

if (NOT TARGET libCppGUI)
    message(FATAL_ERROR "libCppGUI (main library) not defined as a CMake target")
endif()

target_include_directories(${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)
