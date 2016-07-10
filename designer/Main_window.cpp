#include <iostream>
#include <memory>

#include <gpc/fonts/store.hpp>
#include <cppgui/Default_font.hpp>
#include <cppgui/unicode.hpp>

#include "./Main_window.hpp"

// Explicit template instantiations for libCppGUI

#include <cppgui/Default_widgets.ipp>
#include <cppgui/sdl/Window.ipp>
#include <cppgui/GUI_window.ipp>
#include <cppgui/Container.ipp>
#include <cppgui/Slider.ipp>

CPPGUI_SDL_INSTANTIATE_WINDOW(Main_window)
CPPGUI_INSTANTIATE_GUI_WINDOW(Main_window, GUI_config, cppgui::sdl::Window<Main_window>, cppgui::sdl::OpenGL_adapter)

static cppgui::Rasterized_font *dflt_font;
static cppgui::Rasterized_font *glyph_font;     // TODO: move this to a reusable module in cppgui itself

// Class implementation  --------------------------------------------

Main_window::Main_window(): Parent_t("Main window")
{
    using Default_font = GUI_config::Default_font;
    using namespace std::string_literals;

    // TODO: doesn't really belong here (could be executed more than once)
    auto dflt_font_data = Default_font::get();
    dflt_font = gpc::fonts::get(dflt_font_data.first, dflt_font_data.second);

    auto tick_font_data = cppgui::Icon_resources<Default_font::size>::tick_font_data();
    glyph_font = gpc::fonts::get(tick_font_data.first, tick_font_data.second);
    auto tick_descr = cppgui::Icon_resources<Default_font::size>::tick_descr();

    // Left panel

    _stringlist.set_font(dflt_font);
    for (auto i = 1U; i <= 25; i ++)
    {
        _stringlist.add_item( "gee, item #"s + std::to_string(i) );
    }
    _stringlist.on_item_selected([](cppgui::Index index, const std::u32string &item) {
        std::cout << "Item #" << index << " selected: " << cppgui::utf32_to_utf8(item) << std::endl;
    });
    _stringlist.on_item_activated([](cppgui::Index index, const std::u32string &item) {
        std::cout << "Item #" << index << " activated: " << cppgui::utf32_to_utf8(item) << std::endl;
    });
    _left_panel.set_left(&_stringlist, {1, 1});
    _left_panel.set_background_color({0.8f, 0.8f, 0.8f, 1});

    // Right panel

    _right_panel.set_id("RIGHT PANEL");
    _right_panel.set_background_color({0.8f, 0.8f, 0.8f, 1});
    _right_panel.set_spacing(5);
    _slider1.set_font(dflt_font);
    _slider1.slider().define_range({  50, 120 });
    _slider1.set_value( 50 );
    _right_panel.add_element(&_slider1);
    _slider2.set_font(dflt_font);
    _slider2.slider().define_range({  -50, 50 });
    _slider2.set_value( 20 );
    _right_panel.add_element(&_slider2);
    _slider3.set_font(dflt_font);
    _slider3.slider().define_range({  0.001f, 0.010f });
    _slider3.set_value( 0.001f );
    _right_panel.add_element(&_slider3);
    _right_panel.add_spacer(1);

    // Root widget

    _root_widget.set_background_color({ 0, 0.6f, 0.2f, 1 });
    _root_widget.set_left (&_left_panel , {1, 4});
    _root_widget.set_right(&_right_panel, {1, 4});

    _root_widget.on_invalidated([this]() { invalidate(); });

    _root_widget.init_layout();     // layout() and init() will be called upon by the Window
                                    // root_widget()->layout();
                                    // root_widget()->init();

    this->init_window(); // will initialize the GUI and must therefore come last here
}

Main_window::Slider_with_display::Slider_with_display()
{
    _slider.on_value_changed([this](const Value_type &value)
    {
        _textbox.change_text( std::to_string(value) );
    });
    _textbox.on_done([this](const std::u32string &text)
    {
        _slider.change_value( std::stof(cppgui::utf32_to_utf8(text) ) );
    });

    this->set_spacing(5);

    _textbox.set_size(5);

    add_element(&_textbox);
    add_element(&_slider, 1);
    add_spacer(1);
}

void Main_window::Slider_with_display::set_font(const cppgui::Rasterized_font *font)
{
    _textbox.set_font(font);
}

void Main_window::Slider_with_display::set_value(Value_type value)
{
    _textbox.set_text( std::to_string(value) );
    _slider.set_value(value);
}

void Main_window::Slider_with_display::compute_view_from_data()
{
    Parent_t::compute_view_from_data();

    _textbox.set_text( std::to_string( _slider.value() ) );
}
