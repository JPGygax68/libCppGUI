/** This file explicitly instantiates all the template classes contained
 *  in libCppGUI.
 */

#include "./Test_window.hpp"

#include <cppgui/sdl/Application.ipp>
template class cppgui::sdl::Application<Test_window>;

#include <cppgui/Default_widgets.ipp>
#include <cppgui/Default_font.ipp>

CPPGUI_INSTANTIATE_WIDGET_TYPES(GUI_configuration, true)
CPPGUI_INSTANTIATE_DEFAULT_FONT(16)
//CPPGUI_INSTANTIATE_ICON_RESOURCES(16)   // TODO: split icon resources that are needed "big" and those that are needed at font size ?
//CPPGUI_INSTANTIATE_ICON_RESOURCES(24)

