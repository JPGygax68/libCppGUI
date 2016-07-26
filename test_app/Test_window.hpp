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

class Test_window: public cppgui::GUI_window<Test_window, GUI_config, cppgui::sdl::Window<Test_window>, cppgui::sdl::OpenGL_adapter>
{
public:
    using Parent_t = typename cppgui::GUI_window<Test_window, GUI_config, cppgui::sdl::Window<Test_window>, cppgui::sdl::OpenGL_adapter>;
    //CPPGUI_DEFINE_WIDGET_TYPES(GUI_configuration, true);

    Test_window();

    auto root_widget() { return &_root_widget; }

private:
    
    class Slider_with_display: public 
        cppgui::Container<Slider_with_display, GUI_config, true, cppgui::Box_model_definition::run_time,
        cppgui::Single_column_flow_layout<GUI_config, true>>
    {
    public:
        using Value_type = cppgui::Vertical_slider<GUI_config, true>::Value_type;
        using Parent_t = cppgui::Container<Slider_with_display, GUI_config, true, cppgui::Box_model_definition::run_time,
            cppgui::Single_column_flow_layout<GUI_config, true> >;

        constexpr static auto default_padding(int /*dir*/) { return 2; }

        Slider_with_display();

        void set_font(const cppgui::Rasterized_font *);
        void set_value(Value_type);

        void compute_view_from_data() override;

        auto& textbox() { return _textbox; }
        auto& slider () { return _slider; }

    private:
        cppgui::Textbox<GUI_config, true, cppgui::Box_model_definition::run_time>   _textbox;
        cppgui::Vertical_slider<GUI_config, true>                                   _slider;
    };

    class Left_panel: public cppgui::Container< Left_panel, GUI_config, true, 
        cppgui::Box_model_definition::run_time, 
        cppgui::Horizontal_box<GUI_config, true, cppgui::Aspect_injection::by_inheritance> 
    > {};

    class Right_panel: public cppgui::Container< Right_panel, GUI_config, true, 
        cppgui::Box_model_definition::run_time, cppgui::Single_row_flow_layout<GUI_config, true>
    > {
    protected:
        constexpr static auto default_padding(int /*dir*/) { return 2; }
    };

    using Root_widget_t = cppgui::Root_widget<GUI_config, true, cppgui::Box_model_definition::run_time, 
        cppgui::Horizontal_box<GUI_config, true, cppgui::Aspect_injection::by_inheritance> >;
    
    Root_widget_t                                                                               _root_widget;

    Left_panel                                                                                  _left_panel;
    Right_panel                                                                                 _right_panel;

    cppgui::Listbox<GUI_config, true, cppgui::Box_model_definition::run_time>                   _listbox;

    cppgui::Label<GUI_config, true, cppgui::Box_model_definition::run_time>                     _label;
    cppgui::Button<GUI_config, true, cppgui::Box_model_definition::run_time>                    _button;
    cppgui::Glyph_button<GUI_config, true, cppgui::Box_model_definition::run_time>              _glyph_btn;
    cppgui::Glyph_button<GUI_config, true, cppgui::Box_model_definition::run_time>              _button2;
    cppgui::Textbox<GUI_config, true, cppgui::Box_model_definition::run_time>                   _textbox;
    cppgui::Checkbox<GUI_config, true, cppgui::Box_model_definition::run_time>                  _checkbox;
    cppgui::Label<GUI_config, true, cppgui::Box_model_definition::run_time>                     _menu_header;
    //Text_input_dialog<GUI_configuration, true>                 _input_dlg;
    cppgui::Vertical_scrollbar<GUI_config, true, cppgui::Box_model_definition::run_time>        _vert_scrollbar;   
    cppgui::Textbox<GUI_config, true, cppgui::Box_model_definition::run_time>                   _scrollbar_pos;
    cppgui::List_pane_base<GUI_config, true, cppgui::Box_model_definition::run_time>            _sb_pane;
    std::vector<cppgui::Button<GUI_config, true, cppgui::Box_model_definition::run_time>>       _sb_buttons;
    std::vector<cppgui::Button<GUI_config, true, cppgui::Box_model_definition::run_time>>       _lb_buttons;
    cppgui::Stringlist<GUI_config, true, cppgui::Box_model_definition::run_time>                _stringlist;
    Slider_with_display                                                                         _slider1, _slider2, _slider3;
};
