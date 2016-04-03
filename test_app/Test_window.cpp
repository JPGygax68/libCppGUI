#include <iostream>
#include <memory>

#include <gpc/fonts/store.hpp>

#include <cppgui/all_widgets.ipp>
#include <cppgui/sdl/Window.ipp>
#include <cppgui/GUI_window.ipp>

#include "./Fonts.hpp"

#include "all_fonts.h" // TODO: move back to cppgui module

#include "./Test_window.hpp"

// Class implementation  --------------------------------------------

template My_SDL_window; // apparently an explicit instantiation is needed here

static cppgui::Rasterized_font  dflt_font;
static cppgui::Rasterized_font  glyph_font;     // TODO: move this to a reusable module in cppgui itself

Test_window::Test_window(): Parent("Test window")
{
    static const size_t font_size = 14;

    auto dflt_font_data = liberationsans_regular<font_size>::get();
    dflt_font = gpc::fonts::load(dflt_font_data.first, dflt_font_data.second);

    auto tick_font_data = cppgui::Icon_resources<font_size>::tick_font_data();
    glyph_font = gpc::fonts::load(tick_font_data.first, tick_font_data.second);
    auto tick_descr = cppgui::Icon_resources<font_size>::tick_descr();

    _label.set_font(&dflt_font);
    _label.set_text(U"Hello World!");
    _label.on_click([](const cppgui::Position &pos, int button, int clicks) {
        std::cout << "Label was clicked! (pos = " << pos.x << ", " << pos.y 
            << ", button = " << button << ", clicks = " << clicks << ")" << std::endl;
    });
    _label.set_position({ 50, 50 });
    //_label.set_extents({ 0, 0 }); //200, 50 });

    _textbox.set_font(&dflt_font);
    _textbox.set_position({50, 120});
    //_textbox.set_extents({ 200, 30 });
    _textbox.set_text(U"Abc1234567890");
    
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

    root_widget().add_child(&_label);
    root_widget().add_child(&_textbox);
    root_widget().add_child(&_button);
    root_widget().add_child(&_stack);
    root_widget().add_child(&_checkbox);

    _button_list.resize(8);
    for (auto i = 0U; i < _button_list.size(); i ++)
    {
        _button_list[i].set_font(&dflt_font);
        _button_list[i].set_label(std::u32string{U"This is button #"} + char32_t(U'1' + i));
        _stack.add_child(&_button_list[i]);
    }
    _stack.set_position({50, 250});
    _stack.set_extents({200, 207});

    root_widget().set_focus_to(&_textbox);

    root_widget().on_invalidated([this]() { invalidate(); });

    root_widget().init_layout();
    root_widget().layout();
    root_widget().init();
}

