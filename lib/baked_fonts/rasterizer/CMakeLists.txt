cmake_minimum_required(VERSION 3.4)

#------------------------------------------------
# Target: executable
#

project(fontrasterizer)

file(GLOB SOURCES *.cpp)

add_executable(${PROJECT_NAME} ${SOURCES})

#------------------------------------------------
# CMake scripts
#

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/FindFreetype.cmake)
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})

#------------------------------------------------
# Conan
#

if (0)
# Download automatically, you can also just copy the conan.cmake file
if (NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
   message(STATUS "Downloading conan.cmake from https://github.com/memsharded/cmake-conan")
   file(DOWNLOAD "https://raw.githubusercontent.com/conan-io/cmake-conan/master/conan.cmake"
                 "${CMAKE_BINARY_DIR}/conan.cmake")
endif()

include(${CMAKE_BINARY_DIR}/conan.cmake)

conan_cmake_run(REQUIRES freetype/2.6.3@hilborn/stable Boost/1.62.0@lasote/vcpkg
                BASIC_SETUP CMAKE_TARGETS
                OPTIONS Boost:shared=False
                BUILD missing)
endif()

#------------------------------------------------
# Libraries
#

# Cereal (header-only)

if (0)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/external/cereal/include )
endif()

# Boost

if (1)

target_link_libraries(${PROJECT_NAME} PRIVATE CONAN_PKG::Boost)

else()
set(Boost_USE_STATIC_LIBS ON)
find_package(Boost REQUIRED COMPONENTS system filesystem)
target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})
target_include_directories(${PROJECT_NAME} PRIVATE ${Boost_INCLUDE_DIR})
endif()

# FreeType

target_link_libraries(${PROJECT_NAME} PRIVATE CONAN_PKG::freetype)

# Cereal (header-only library)

target_link_libraries(${PROJECT_NAME} PUBLIC CONAN_PKG::cereal)


#-----------------------------------------------
# Testing
#

enable_testing()
