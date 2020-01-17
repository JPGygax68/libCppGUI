project(cppgui_platform_sdl2)

if (0)
# Download automatically, you can also just copy the conan.cmake file
if (NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
   message(STATUS "Downloading conan.cmake from https://github.com/memsharded/cmake-conan")
   file(DOWNLOAD "https://raw.githubusercontent.com/conan-io/cmake-conan/master/conan.cmake"
                 "${CMAKE_BINARY_DIR}/conan.cmake")
endif()

include(${CMAKE_BINARY_DIR}/conan.cmake)

conan_cmake_run(REQUIRES SDL2/2.0.10@lasote/stable
                BASIC_SETUP CMAKE_TARGETS
                #OPTIONS SDL2:build_type=Debug
                BUILD missing)
endif()

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

if (TARGET SDL2)
    target_link_libraries(${PROJECT_NAME} PUBLIC SDL2)
elseif(TARGET CONAN_PKG::SDL2)
    target_link_libraries(${PROJECT_NAME} PUBLIC CONAN_PKG::SDL2)
else()
    message(FATAL_ERROR "SDL2 target unavailable, either directly or from Conan")
    #target_link_libraries(${PROJECT_NAME} PUBLIC SDL2-static)
endif()




