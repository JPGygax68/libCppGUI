#pragma once

#include <vector>

#pragma warning(push)
#pragma warning(disable:4251)
#include <glbinding/gl/gl.h>
#include <glbinding/Binding.h>
#pragma warning(pop)
using namespace gl;

#include <gpc/gui/gl/renderer.hpp> // TODO: replace with new namespace/naming
#include <cppgui/all_widgets.hpp>
#include <cppgui/GUI_window.hpp>

#include <cppgui/sdl/Window.hpp>
#include <cppgui/sdl/Default_configuration.hpp>
#include <cppgui/sdl/OpenGL_adapter.hpp>

#include "./Text_input_dialog.hpp"

using Renderer = typename gpc::gui::gl::renderer<true>;
struct GUI_configuration: cppgui::sdl::Default_configuration<GUI_configuration, Renderer, true> {

    using Default_font = typename cppgui::Default_font<16>;
};

class Test_window;
using My_SDL_window = cppgui::sdl::Window<Test_window>;
class Test_window;
using My_GUI_window = cppgui::GUI_window<Test_window, GUI_configuration, My_SDL_window, cppgui::sdl::OpenGL_adapter>;

using Text_input_dialog_t = typename Text_input_dialog<GUI_configuration, true>;

#ifdef INSTANTIATE_GUI_CLASSES
template cppgui::All_widgets<GUI_configuration, true>;
#else
extern template cppgui::All_widgets<GUI_configuration, true>;
extern template cppgui::All_widgets<GUI_configuration, true>::Label;
#endif

class Test_window: public My_GUI_window {
public:
    using Parent = My_GUI_window;
    //CPPGUI_DEFINE_WIDGET_TYPES(GUI_configuration, true);

    using Widgets = cppgui::All_widgets<GUI_configuration, true>;

    Test_window();

private:
    using My_listbox = cppgui::Listbox<GUI_configuration, true, true>;

    Widgets::Label                  _label;
    Widgets::Button                 _button;
    Widgets::Glyph_button           _glyph_btn;
    Widgets::Glyph_button           _button2;
    Widgets::Textbox                _textbox;
    Widgets::Checkbox               _checkbox;
    Widgets::Container              _menu;
    Widgets::Label                  _menu_header;
    Text_input_dialog_t             _input_dlg;
    Widgets::Vertical_scrollbar     _vert_scrollbar;   
    Widgets::Textbox                _scrollbar_pos;
    std::vector<Widgets::Button>    _lb_buttons;
    My_listbox                      _listbox;
};
