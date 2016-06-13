# This file makes dependencies obtained via the Conan package manager available to CMake.
# It is intended to be include()'ed from the top-level CMakeLists.txt of a consuming project.

# The GPC Font Rasterizer is needed to pre-rasterize fonts
add_executable(gpcfontrasterizer IMPORTED)
set_target_properties(gpcfontrasterizer PROPERTIES 
  IMPORTED_LOCATION "${CMAKE_BINARY_DIR}/bin/gpcfontrasterizer"   # find it in the default subdirectory for Conan-imported binaries
)
