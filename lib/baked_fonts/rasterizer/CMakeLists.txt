cmake_minimum_required(VERSION 3.4)

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/FindFreetype.cmake)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})

#------------------------------------------------
# Target: executable
#

project(fontrasterizer)

file(GLOB SOURCES *.cpp)

add_executable(${PROJECT_NAME} ${SOURCES})

#------------------------------------------------
# Libraries
#

# Cereal (header-only)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/external/cereal/include )

# Boost

set(Boost_USE_STATIC_LIBS ON)
find_package(Boost REQUIRED COMPONENTS system filesystem)
target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})
target_include_directories(${PROJECT_NAME} PRIVATE ${Boost_INCLUDE_DIR})

# FreeType

if (0)
# FreeType: get the one we built as part of our prereqs

set(ENV{FREETYPE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/../prereqs/install/freetype)
find_package(Freetype REQUIRED)
target_link_libraries(${PROJECT_NAME} ${FREETYPE_LIBRARIES})
target_include_directories(${PROJECT_NAME} PRIVATE ${FREETYPE_INCLUDE_DIRS})
#target_link_libraries(${PROJECT_NAME} freetype)

else()

add_subdirectory(external/freetype2)
include_directories(external/freetype2/include)
target_link_libraries(${PROJECT_NAME} freetype)

endif()

# Also required for FreeType
add_subdirectory("external/zlib")
if (NOT TARGET zlib)
    message(FATAL_ERROR "zlib is not defined as a target")
endif()
target_link_libraries(${PROJECT_NAME} ${ZLIB_LIBRARIES})
target_include_directories(${PROJECT_NAME} PRIVATE ${ZLIB_INCLUDE_DIRS})

#-----------------------------------------------
# Testing
#

enable_testing()
