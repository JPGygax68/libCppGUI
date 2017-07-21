project (CppGUI)

cmake_minimum_required(VERSION 3.4)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

if (0)
find_path(CEREAL_DIR "cereal/cereal.hpp")
if (NOT CEREAL_DIR)
    include_directories("external/cereal/include")
    message("Cereal: not found, adding include directory from submodule")
else()
    message("Cereal already available, adding directory \"${CEREAL_HPP}\"")
    include_directories(${CEREAL_DIR})
endif()
endif()

#if (NOT TARGET gpcbin2c)
#    add_subdirectory(external/GPCBin2C)
#endif()

add_subdirectory(lib)

option(LIBCPPGUI_BUILD_TESTAPP "Build libCppGUI test application" ${LIBCPPGUI_BUILD_TESTAPP})
if (LIBCPPGUI_BUILD_TESTAPP)
    add_subdirectory(test_app)
endif()
