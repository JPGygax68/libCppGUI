project(cppgui_platform_sdl2)

# Download automatically, you can also just copy the conan.cmake file
if (NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
   message(STATUS "Downloading conan.cmake from https://github.com/memsharded/cmake-conan")
   file(DOWNLOAD "https://raw.githubusercontent.com/conan-io/cmake-conan/master/conan.cmake"
                 "${CMAKE_BINARY_DIR}/conan.cmake")
endif()

include(${CMAKE_BINARY_DIR}/conan.cmake)

conan_cmake_run(REQUIRES SDL2/2.0.5@lasote/stable
                BASIC_SETUP CMAKE_TARGETS
                #OPTIONS SDL2:build_type=Debug
                BUILD missing)

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

if (0)
if (NOT TARGET SDL2-static)
    message(FATAL_ERROR "SDL2 not defined as a target")
    # TODO: fallback to looking for module ? or let conan handle it ?
endif()
target_link_libraries(${PROJECT_NAME} PUBLIC SDL2-static)
else()
# TODO: support using the static build of SDL2
target_link_libraries(${PROJECT_NAME} PUBLIC CONAN_PKG::SDL2)
endif()




