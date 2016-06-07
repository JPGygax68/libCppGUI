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

using Text_input_dialog_t = typename Text_input_dialog<GUI_configuration, true>;

class Test_window: public cppgui::GUI_window<Test_window, GUI_configuration, cppgui::sdl::Window<Test_window>, cppgui::sdl::OpenGL_adapter>
{
public:
    using Parent_t = typename cppgui::GUI_window<Test_window, GUI_configuration, cppgui::sdl::Window<Test_window>, cppgui::sdl::OpenGL_adapter>;
    CPPGUI_DEFINE_WIDGET_TYPES(GUI_configuration, true);

    Test_window();

private:
    Label                       _label;
    Button                      _button;
    Glyph_button                _glyph_btn;
    Glyph_button                _button2;
    Textbox                     _textbox;
    Checkbox                    _checkbox;
    Container                   _menu;
    Label                       _menu_header;
    Text_input_dialog_t         _input_dlg;
    Vertical_scrollbar          _vert_scrollbar;   
    Textbox                     _scrollbar_pos;
    List_pane_base              _sb_pane;
    std::vector<Button>         _sb_buttons;
    std::vector<Button>         _lb_buttons;
    Listbox                     _listbox;
    Stringlist                  _stringlist;
    Vertical_slider             _vslider;

    Container                   _container1;
};
