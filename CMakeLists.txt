project (CppGUI)

cmake_minimum_required(VERSION 3.4)

add_subdirectory(external/SDL2)
include_directories(external/SDL2/include)

add_subdirectory(lib)

add_subdirectory(test_app)
