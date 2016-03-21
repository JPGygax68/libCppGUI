project (libCppGUI)

add_library( ${PROJECT_NAME} STATIC

    src/dummy.cpp

    include/cppgui/Widget.hpp                       include/cppgui/Widget.ipp
    include/cppgui/Label.hpp                        include/cppgui/Label.ipp
    include/cppgui/Textbox_base.hpp                      include/cppgui/Textbox_base.ipp
    include/cppgui/Box.hpp                          include/cppgui/Box.ipp
    include/cppgui/Container.hpp                    include/cppgui/Container.ipp
    include/cppgui/Root_widget.hpp                  include/cppgui/Root_widget.ipp
    include/cppgui/Resource_mapper.hpp              include/cppgui/Resource_mapper.ipp
    include/cppgui/Cheap_resource_mapper.hpp        include/cppgui/Cheap_resource_mapper.ipp
    include/cppgui/Full_resource_mapper.hpp         include/cppgui/Full_resource_mapper.ipp
    include/cppgui/aspects.hpp
)

if (NOT TARGET libGPCFonts)
    message(FATAL_ERROR "libGPCFonts not defined as a CMake target")
endif()

target_include_directories(${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)