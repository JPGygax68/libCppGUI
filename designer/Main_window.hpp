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
#include <cppgui/Slider.hpp>

#include <cppgui/sdl/Window.hpp>
#include <cppgui/sdl/Default_configuration.hpp>
#include <cppgui/sdl/OpenGL_adapter.hpp>

//#include "./Text_input_dialog.hpp"

using Renderer = typename gpc::gui::gl::renderer<true>;
struct GUI_config: cppgui::sdl::Default_configuration<GUI_config, Renderer, true> {

    using Default_font = typename cppgui::Default_font<16>;
};

//using Text_input_dialog_t = typename Text_input_dialog<GUI_configuration, true>;

class Main_window: public cppgui::GUI_window<Main_window, GUI_config, cppgui::sdl::Window<Main_window>, cppgui::sdl::OpenGL_adapter>
{
public:
    using Parent_t = typename cppgui::GUI_window<Main_window, GUI_config, cppgui::sdl::Window<Main_window>, cppgui::sdl::OpenGL_adapter>;
    //CPPGUI_DEFINE_WIDGET_TYPES(GUI_configuration, true);

    Main_window();

    auto root_widget() { return &_root_widget; }

private:
    
    // TODO: use even simpler box model

    class Slider_with_display: public cppgui::Container<GUI_config, true, 
        cppgui::Fixed_border_and_padding<0, 0>::Box_model, cppgui::Single_column_flow_layout<GUI_config, true>>
    {
    public:
        using Value_type = cppgui::Vertical_slider<GUI_config, true>::Value_type;
        using Parent_t = cppgui::Container<GUI_config, true, cppgui::Fixed_border_and_padding<0, 0>::Box_model, 
            cppgui::Single_column_flow_layout<GUI_config, true>>;

        Slider_with_display();

        void set_font(const cppgui::Rasterized_font *);
        void set_value(Value_type);

        void compute_view_from_data() override;


        auto& textbox() { return _textbox; }
        auto& slider () { return _slider; }


    private:
        cppgui::Textbox<GUI_config, true>            _textbox;
        cppgui::Vertical_slider<GUI_config, true>    _slider;
    };

    template<class Layout>
    using Panel = cppgui::Container<GUI_config, true, cppgui::Fixed_border_and_padding<1, 10>::Box_model, Layout>;
    using Single_line_layout = cppgui::Single_row_flow_layout<GUI_config, true>;
    using Horizontal_box_layout = cppgui::Horizontal_box<GUI_config, true>;

    cppgui::Root_widget<GUI_config, true, cppgui::Simple_box_model, cppgui::Horizontal_box<GUI_config, true>> _root_widget;

    Panel<Horizontal_box_layout>                        _left_panel;
    Panel<Single_line_layout>                           _right_panel;

    cppgui::Listbox<GUI_config, true>                   _listbox;

    cppgui::Label<GUI_config, true>                     _label;
    cppgui::Button<GUI_config, true>                    _button;
    cppgui::Glyph_button<GUI_config, true>              _glyph_btn;
    cppgui::Glyph_button<GUI_config, true>              _button2;
    cppgui::Textbox<GUI_config, true>                   _textbox;
    cppgui::Checkbox<GUI_config, true>                  _checkbox;
    cppgui::Label<GUI_config, true>                     _menu_header;
    //Text_input_dialog<GUI_configuration, true>                 _input_dlg;
    cppgui::Vertical_scrollbar<GUI_config, true>        _vert_scrollbar;   
    cppgui::Textbox<GUI_config, true>                   _scrollbar_pos;
    cppgui::List_pane_base<GUI_config, true>            _sb_pane;
    std::vector<cppgui::Button<GUI_config, true>>       _sb_buttons;
    std::vector<cppgui::Button<GUI_config, true>>       _lb_buttons;
    cppgui::Stringlist<GUI_config, true>                _stringlist;
    Slider_with_display                                 _slider1, _slider2, _slider3;

    //cppgui::Container<GUI_config, true, cppgui::Fixed_border_and_padding<1, 3>::Box_model>  _menu;
    //Vertical_slider             _vslider1, _vslider2, _vslider3;
    //cppgui::Container<GUI_config, true, cppgui::Fixed_border_and_padding<1, 3>::Box_model>   _container1, _container2;
};
