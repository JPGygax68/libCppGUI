#include <iostream>
#include <memory>

#include <gpc/fonts/store.hpp>
#include <cppgui/Default_font.hpp>

#include "./Test_window.hpp"

// Class implementation  --------------------------------------------

#include <cppgui/sdl/Window.ipp>
#include <cppgui/GUI_window.ipp>

CPPGUI_SDL_INSTANTIATE_WINDOW(Test_window)
CPPGUI_INSTANTIATE_GUI_WINDOW(Test_window, GUI_configuration, cppgui::sdl::Window<Test_window>, cppgui::sdl::OpenGL_adapter)

static cppgui::Rasterized_font *dflt_font;
static cppgui::Rasterized_font *glyph_font;     // TODO: move this to a reusable module in cppgui itself

Test_window::Test_window(): Parent_t("Test window")
{
    using Default_font = GUI_configuration::Default_font;
    using namespace std::string_literals;

    // TODO: doesn't really belong here (could be executed more than once)
    auto dflt_font_data = Default_font::get();
    dflt_font = gpc::fonts::get(dflt_font_data.first, dflt_font_data.second);

    auto tick_font_data = cppgui::Icon_resources<Default_font::size>::tick_font_data();
    glyph_font = gpc::fonts::get(tick_font_data.first, tick_font_data.second);
    auto tick_descr = cppgui::Icon_resources<Default_font::size>::tick_descr();

    _label.set_font(dflt_font);
    _label.set_background_color({1, 1, 1, 1});
    _label.set_text(U"Hello World!");
    _label.on_click([](const cppgui::Point &pos, int button, int clicks) {

        std::cout << "Label was clicked! (pos = " << pos.x << ", " << pos.y 
            << ", button = " << button << ", clicks = " << clicks << ")" << std::endl;
    });
    _label.set_position({  50, 50 });
    _label.set_extents ({ 200, 50 });

    _textbox.set_font(dflt_font);
    _textbox.set_position({50, 120});
    //_textbox.set_extents({ 200, 30 });
    _textbox.set_text(U"Abc1234567890");

    _button.set_font(dflt_font);
    _button.set_position({  50, 160 });
    _button.set_extents ({ 200,  30 });
    _button.set_label(U"Click Me!");

    _button2.set_font(dflt_font);
    _button2.set_label(U"Decorated btn");
    _button2.set_glyph(cppgui::Icon_resources<Default_font::size>::close());
    _button2.set_position({50, 195});
    _button2.set_extents({200, 30});

    _stringlist.set_font(dflt_font);
    _stringlist.set_position({ 50, 240 });
    _stringlist.set_extents ({200, 395 });
    for (auto i = 1U; i <= 25; i ++)
    {
        _stringlist.add_item( "gee, item #"s + std::to_string(i) );
    }

    _glyph_btn.set_glyph(cppgui::Icon_resources<Default_font::size>::close());
    _glyph_btn.set_position({280, 160});
    _glyph_btn.set_extents({30, 30});

    _checkbox.set_font( dflt_font );
    _checkbox.set_tick_glyph( glyph_font, tick_descr );
    _checkbox.set_position({ 350, 50 });
    _checkbox.set_extents ({ 200, 40 });
    _checkbox.set_label( U"Check me!" );

    /*
    _button_list.resize(9);
    for (auto i = 0U; i < _button_list.size(); i ++)
    {
    _button_list[i].set_font(dflt_font);
    _button_list[i].set_label("This is button #"s + std::to_string(i + 1));
    _stack.add_child(&_button_list[i]);
    }
    */

    _menu_header.set_font(dflt_font);
    _menu_header.set_text(U"Look at this:");
    _menu_header.set_background_color({ 1, 1, 1, 1 });
    //_menu_header.set_padding({ 3, 3, 3, 3 });
    _menu.set_layout_type(cppgui::Layout_type::header_content);
    _menu.add_child(&_menu_header);
    //_menu.add_child(&_stack);
    _menu.set_position({  50, 250 });
    _menu.set_extents ({ 200, 207 });

    _input_dlg.set_default_font(dflt_font);
    _input_dlg.set_caption(U"Give me a value!");
    _input_dlg.set_caption_font(dflt_font);
    _input_dlg.set_prompt(U"Please enter the captain's age:");
    _input_dlg.set_position({ 350, 250 });
    _input_dlg.set_extents ({ 350, 180 });

    _vert_scrollbar.set_position({ 750,  50 });
    _vert_scrollbar.set_extents ({  30, 200 });
    _vert_scrollbar.define_values( 150, 40 );
    _vert_scrollbar.on_position_change([&](cppgui::Position pos) { _scrollbar_pos.change_text( std::to_string(pos) ); });

    _scrollbar_pos.set_font(dflt_font);
    _scrollbar_pos.set_position({ 800, 50 });
    //_scrollbar_pos.set_extents ({ 100, 30 });

    // Listbox
    _lb_buttons.resize(25);
    for (auto i = 0U; i < _lb_buttons.size(); i ++)
    {
        _lb_buttons[i].set_font(dflt_font);
        _lb_buttons[i].set_label("Scrollbox button #"s + std::to_string(i + 1));
        _listbox.add_item(&_lb_buttons[i]);
    }
    _listbox.set_position({ 800,  90 });
    _listbox.set_extents ({ 400, 120 });
    _listbox.content_pane()->set_item_padding({ 5, 5 });
    _listbox.content_pane()->set_separator({1, {0.5f, 0.5f, 0.5f, 1}});

    root_widget()->set_background_color({0, 0.6f, 0.2f, 1});

    root_widget()->add_child(&_label);
    root_widget()->add_child(&_textbox);
    root_widget()->add_child(&_button);
    root_widget()->add_child(&_button2);
    root_widget()->add_child(&_stringlist);
    root_widget()->add_child(&_glyph_btn);
    root_widget()->add_child(&_checkbox);
    //root_widget()->add_child(&_menu);
    root_widget()->add_child(&_input_dlg);
    root_widget()->add_child(&_vert_scrollbar);
    root_widget()->add_child(&_scrollbar_pos);
    ////root_widget()->add_child(&_scrollbox);
    root_widget()->add_child(&_listbox);

    //_menu.take_focus();
    //root_widget()->set_focus_to(&_menu); // ); // &_textbox);
    //_scrollbox.take_focus();
    //_listbox.take_focus();

    root_widget()->on_invalidated([this]() { invalidate(); });

    root_widget()->init_layout(); // layout() and init() will be called upon by the Window
                                  //root_widget()->layout();
                                  //root_widget()->init();

    init_window(); // will initialize the GUI and must therefore come last here
}
