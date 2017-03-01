@echo off
::del build /s /q
mkdir build >nul
cd build
set CMAKE_SEARCH_PATH=../../prereqs/install
cmake-gui ..