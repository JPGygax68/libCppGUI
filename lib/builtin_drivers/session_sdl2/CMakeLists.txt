project(cppgui_session_sdl2)

add_library(${PROJECT_NAME} STATIC 
    src/session_sdl2.cpp include/cppgui/builtin_drivers/session_sdl2.hpp
)
    
# Note: this library is intended to be used as a CMake target only
target_include_directories(${PROJECT_NAME} BEFORE PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# External dependencies

if (NOT TARGET SDL2)
    message(FATAL_ERROR "SDL2 not defined as a target")
# TODO: fallback to looking for module ? or let conan handle it ?
endif()
target_link_libraries(${PROJECT_NAME} PUBLIC SDL2)




