#include <iostream>
#include <memory>

#include <gpc/fonts/store.hpp>

#include <cppgui/Default_font.hpp>

#include <cppgui/all_widgets.ipp>
#include <cppgui/sdl/Window.ipp>
#include <cppgui/GUI_window.ipp>

#include "./Text_input_dialog.ipp"

#include "./Test_window.hpp"

// Class implementation  --------------------------------------------

template My_SDL_window; // apparently an explicit instantiation is needed here

static cppgui::Rasterized_font *dflt_font;
static cppgui::Rasterized_font *glyph_font;     // TODO: move this to a reusable module in cppgui itself

Test_window::Test_window(): Parent("Test window")
{
    using Default_font = GUI_configuration::Default_font;

    init_window();

    // TODO: doesn't really belong here (could be executed more than once)
    auto dflt_font_data = Default_font::get();
    dflt_font = gpc::fonts::get(dflt_font_data.first, dflt_font_data.second);

    auto tick_font_data = cppgui::Icon_resources<Default_font::size>::tick_font_data();
    glyph_font = gpc::fonts::get(tick_font_data.first, tick_font_data.second);
    auto tick_descr = cppgui::Icon_resources<Default_font::size>::tick_descr();

    _label.set_font(dflt_font);
    _label.set_background_color({1, 1, 1, 1});
    _label.set_text(U"Hello World!");
    _label.on_click([](const cppgui::Position &pos, int button, int clicks) {
        std::cout << "Label was clicked! (pos = " << pos.x << ", " << pos.y 
            << ", button = " << button << ", clicks = " << clicks << ")" << std::endl;
    });
    _label.set_position({ 50, 50 });
    _label.set_extents({ 200, 50 });

    _textbox.set_font(dflt_font);
    _textbox.set_position({50, 120});
    //_textbox.set_extents({ 200, 30 });
    _textbox.set_text(U"Abc1234567890");
    
    _button.set_font(dflt_font);
    _button.set_position({50, 160});
    _button.set_extents({200, 30});
    _button.set_label(U"Click Me!");

    _button2.set_font(dflt_font);
    _button2.set_label(U"Decorated btn");
    _button2.set_glyph(cppgui::Icon_resources<Default_font::size>::close());
    _button2.set_position({50, 195});
    _button2.set_extents({200, 30});

    _glyph_btn.set_glyph(cppgui::Icon_resources<Default_font::size>::close());
    _glyph_btn.set_position({280, 160});
    _glyph_btn.set_extents({30, 30});

    _checkbox.set_font(dflt_font);
    //_checkbox.set_glyph_font(dflt_font);
    //_checkbox.set_glyph_font_size(22);
    _checkbox.set_tick_glyph(glyph_font, tick_descr);
    _checkbox.set_position({350, 50});
    _checkbox.set_extents({200, 40});
    _checkbox.set_label(U"Check me!");

    _button_list.resize(8);
    for (auto i = 0U; i < _button_list.size(); i ++)
    {
        _button_list[i].set_font(dflt_font);
        _button_list[i].set_label(std::u32string{U"This is button #"} + char32_t(U'1' + i));
        _stack.add_child(&_button_list[i]);
    }

    _menu_header.set_font(dflt_font);
    _menu_header.set_text(U"Look at this:");
    _menu_header.set_background_color({1, 1, 1, 1});
    _menu_header.set_padding({3, 3, 3, 3});
    _menu.set_layout_type(cppgui::Layout_type::header_content);
    _menu.add_child(&_menu_header);
    _menu.add_child(&_stack);
    _menu.set_position({50, 250});
    _menu.set_extents ({200, 207});

    _input_dlg.set_default_font(dflt_font);
    _input_dlg.set_caption(U"Give me a value!");
    _input_dlg.set_caption_font(dflt_font);
    _input_dlg.set_prompt(U"Please enter the captain's age:");
    _input_dlg.set_position({350, 250});
    _input_dlg.set_extents ({350, 180});

    root_widget()->set_background_color({0, 0.6f, 0.2f, 1});

    root_widget()->add_child(&_label);
    root_widget()->add_child(&_textbox);
    root_widget()->add_child(&_button);
    root_widget()->add_child(&_button2);
    root_widget()->add_child(&_glyph_btn);
    root_widget()->add_child(&_checkbox);
    root_widget()->add_child(&_menu);
    root_widget()->add_child(&_input_dlg);

    root_widget()->set_focus_to(&_textbox);

    root_widget()->on_invalidated([this]() { invalidate(); });

    root_widget()->init_layout();
    root_widget()->layout();
    root_widget()->init();
}
