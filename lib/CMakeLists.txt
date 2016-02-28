project (libCppGUI)

add_library( ${PROJECT_NAME} STATIC

    src/dummy.cpp

    include/cppgui/Root_widget.hpp include/cppgui/Root_widget.ipp
)

target_include_directories(${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)