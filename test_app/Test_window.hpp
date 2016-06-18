#pragma once

#include <vector>

#pragma warning(push)
#pragma warning(disable:4251)
#include <glbinding/gl/gl.h>
#include <glbinding/Binding.h>
#pragma warning(pop)
using namespace gl;

#include <gpc/gui/gl/renderer.hpp> // TODO: replace with new namespace/naming
#include <cppgui/Default_widgets.hpp>
#include <cppgui/GUI_window.hpp>
#include <cppgui/Container_base.hpp>
#include <cppgui/Vertical_slider.hpp>

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
    //CPPGUI_DEFINE_WIDGET_TYPES(GUI_configuration, true);

    Test_window();

private:
    
    class Slider_with_display: public cppgui::Container_base<GUI_configuration, true>
    {
    public:
        using Value = cppgui::Vertical_slider<GUI_configuration, true>::Value;
        using Parent_t = Container_base;

        Slider_with_display();

        void compute_view_from_data() override;

        auto& textbox() { return _textbox; }
        auto& slider () { return _slider; }

    private:
        cppgui::Textbox<GUI_configuration, true>            _textbox;
        cppgui::Vertical_slider<GUI_configuration, true>    _slider;
    };

    cppgui::Label<GUI_configuration, true>                      _label;
    cppgui::Button<GUI_configuration, true>                     _button;
    cppgui::Glyph_button<GUI_configuration, true>               _glyph_btn;
    cppgui::Glyph_button<GUI_configuration, true>               _button2;
    cppgui::Textbox<GUI_configuration, true>                    _textbox;
    cppgui::Checkbox<GUI_configuration, true>                   _checkbox;
    cppgui::Container_base<GUI_configuration, true>             _menu;
    cppgui::Label<GUI_configuration, true>                      _menu_header;
    Text_input_dialog<GUI_configuration, true>                  _input_dlg;
    cppgui::Vertical_scrollbar<GUI_configuration, true>         _vert_scrollbar;   
    cppgui::Textbox<GUI_configuration, true>                    _scrollbar_pos;
    cppgui::List_pane_base<GUI_configuration, true>             _sb_pane;
    std::vector<cppgui::Button<GUI_configuration, true>>        _sb_buttons;
    std::vector<cppgui::Button<GUI_configuration, true>>        _lb_buttons;
    cppgui::Listbox<GUI_configuration, true>                    _listbox;
    cppgui::Stringlist<GUI_configuration, true>                 _stringlist;
    Slider_with_display                                         _slider1, _slider2, _slider3;
    //Vertical_slider             _vslider1, _vslider2, _vslider3;

    cppgui::Container_base<GUI_configuration, true>              _container1, _container2;
};
