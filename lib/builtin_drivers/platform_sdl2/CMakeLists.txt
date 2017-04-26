project(cppgui_platform_sdl2)

add_library(${PROJECT_NAME} STATIC 
    src/platform_sdl2.cpp           include/cppgui/builtin_drivers/platform_sdl2.hpp
    src/SDL2_window.cpp             include/cppgui/builtin_drivers/SDL2_window.hpp
                                    include/cppgui/builtin_drivers/SDL2_exception.hpp
    src/custom_events.cpp           include/cppgui/builtin_drivers/sdl2_custom_events.hpp
)
    
# Note: this library is intended to be used as a CMake target only
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/cppgui/builtin_drivers/)
target_include_directories(${PROJECT_NAME} BEFORE PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# TODO: configuration file

# External dependencies

if (NOT TARGET SDL2-static)
    message(FATAL_ERROR "SDL2 not defined as a target")
# TODO: fallback to looking for module ? or let conan handle it ?
endif()
target_link_libraries(${PROJECT_NAME} PUBLIC SDL2-static)




