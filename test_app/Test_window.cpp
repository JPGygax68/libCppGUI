#include <iostream>
#include <memory>
#include <cppgui/baked_fonts.hpp>
//#include <cppgui/Default_font.hpp>
//#include <cppgui/all_widgets.ipp>
//#include <cppgui/sdl/Window.ipp>
//#include <cppgui/GUI_window.ipp>
//#include "./Fonts.hpp"
#include "./Test_window.hpp"


// Class implementation  --------------------------------------------

//static cppgui::Rasterized_font  dflt_font;
//static cppgui::Rasterized_font  glyph_font;     // TODO: move this to a reusable module in cppgui itself

Test_window::Test_window(): cppgui::Window("Test window")
{
    //auto tick_font_data = cppgui::Icon_resources<Default_font::size>::tick_font_data();
    //glyph_font = gpc::fonts::load(tick_font_data.first, tick_font_data.second);
    //auto tick_descr = cppgui::Icon_resources<Default_font::size>::tick_descr();

    set_background_color({0, 0.5, 1, 1});

    _label.set_font(&cppgui::baked_fonts::default_font());
    _label.set_color({1, 1, 1, 1});
    //_label.set_background_color({1, 1, 1, 1});
    _label.set_text(U"Hello World!");
    _label.on_click([](const cppgui::Point &pos, int button, int clicks) {
        std::cout << "Label was clicked! (pos = " << pos.x << ", " << pos.y 
            << ", button = " << button << ", clicks = " << clicks << ")" << std::endl;
    });
    _label.set_bounds({ 50, 50 }, _label.get_minimal_bounds());
    root_widget().add_child(&_label);

    /*
    _textbox.set_font(&cppgui::baked_fonts::default_font());
    _textbox.set_bounds({ 100, 50 }, _textbox.get_minimal_bounds());
    //_textbox.set_extents({ 200, 30 });
    _textbox.set_text(U"Abc1234567890");
    */

    #ifdef NOT_DEFINED

    _button.set_font(&dflt_font);
    _button.set_position({50, 160});
    _button.set_extents({200, 30});
    _button.set_label(U"Click Me!");

    _checkbox.set_font(&dflt_font);
    //_checkbox.set_glyph_font(dflt_font);
    //_checkbox.set_glyph_font_size(22);
    _checkbox.set_tick_glyph(&glyph_font, tick_descr);
    _checkbox.set_position({500, 50});
    _checkbox.set_extents({200, 40});
    _checkbox.set_label(U"Check me!");

    _button_list.resize(8);
    for (auto i = 0U; i < _button_list.size(); i ++)
    {
        _button_list[i].set_font(&dflt_font);
        _button_list[i].set_label(std::u32string{U"This is button #"} + char32_t(U'1' + i));
        _stack.add_child(&_button_list[i]);
    }

    _menu_header.set_font(&dflt_font);
    _menu_header.set_text(U"Look at this:");
    _menu_header.set_background_color({1, 1, 1, 1});
    _menu_header.set_padding({3, 3, 3, 3});
    _menu.set_layout_type(cppgui::Layout_type::header_content);
    _menu.add_child(&_menu_header);
    _menu.add_child(&_stack);
    _menu.set_position({50, 250});
    _menu.set_extents({200, 207});

    _input_dlg.set_position({300, 50});
    _input_dlg.set_extents({300, 180});

    root_widget().add_child(&_label);
    root_widget().add_child(&_textbox);
    root_widget().add_child(&_button);
    root_widget().add_child(&_checkbox);
    root_widget().add_child(&_menu);
    root_widget().add_child(&_input_dlg);

    #endif

    root_widget().switch_focused_child(&_label);
}
