project (CppGUI)

cmake_minimum_required(VERSION 3.4)

# Download automatically, you can also just copy the conan.cmake file
if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
   message(STATUS "Downloading conan.cmake from https://github.com/memsharded/cmake-conan")
   file(DOWNLOAD "https://raw.githubusercontent.com/conan-io/cmake-conan/master/conan.cmake"
                 "${CMAKE_BINARY_DIR}/conan.cmake")
endif()

include(${CMAKE_BINARY_DIR}/conan.cmake)

conan_cmake_run(REQUIRES SDL2/2.0.5@lasote/stable
                OPTIONS shared=True
                BASIC_SETUP CMAKE_TARGETS
                BUILD missing)

add_executable(main main.cpp)
target_link_libraries(main ${CONAN_LIBS})

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

find_path(CEREAL_DIR "cereal/cereal.hpp")
if (NOT CEREAL_DIR)
    include_directories("external/cereal/include")
    message("Cereal: not found, adding include directory from submodule")
else()
    message("Cereal already available, adding directory \"${CEREAL_HPP}\"")
    include_directories(${CEREAL_DIR})
endif()

if (0)
add_subdirectory(external/SDL2)
include_directories(external/SDL2/include) # TODO: really necessary ?
endif()


#add_subdirectory(external/glbinding)
#add_subdirectory(external/libGPCGLWrappers/lib)
#add_subdirectory(external/libGPCFonts)

if (NOT TARGET gpcbin2c)
    add_subdirectory(external/GPCBin2C)
endif()

#add_subdirectory(external/GPCFontRasterizer/rasterizer)
#add_subdirectory(external/libGPCGUIRenderer/lib)
#add_subdirectory(external/libGPCGLUtils/lib)
#add_subdirectory(external/libGPCGUIGLRenderer/lib)

add_subdirectory(lib)

#add_subdirectory(sdl)

add_subdirectory(test_app)

#add_subdirectory(designer)
