#include <iostream>
#include <memory>

#include <gpc/fonts/store.hpp>
#include <cppgui/Default_font.hpp>
#include <cppgui/unicode.hpp>

#include "./Test_window.hpp"

// Explicit template instantiations for libCppGUI

#include <cppgui/Default_widgets.ipp>
#include <cppgui/sdl/Window.ipp>
#include <cppgui/GUI_window.ipp>
#include <cppgui/Container.ipp>

CPPGUI_SDL_INSTANTIATE_WINDOW(Test_window)
CPPGUI_INSTANTIATE_GUI_WINDOW(Test_window, GUI_config, cppgui::sdl::Window<Test_window>, cppgui::sdl::OpenGL_adapter)

//#include "./Text_input_dialog.ipp"
//template Text_input_dialog<GUI_config, true>;

static cppgui::Rasterized_font *dflt_font;
static cppgui::Rasterized_font *glyph_font;     // TODO: move this to a reusable module in cppgui itself

// Class implementation  --------------------------------------------

Test_window::Test_window(): Parent_t("Test window")
{
    using Default_font = GUI_config::Default_font;
    using namespace std::string_literals;

    // TODO: doesn't really belong here (could be executed more than once)
    auto dflt_font_data = Default_font::get();
    dflt_font = gpc::fonts::get(dflt_font_data.first, dflt_font_data.second);

    auto tick_font_data = cppgui::Icon_resources<Default_font::size>::tick_font_data();
    glyph_font = gpc::fonts::get(tick_font_data.first, tick_font_data.second);
    auto tick_descr = cppgui::Icon_resources<Default_font::size>::tick_descr();

    #ifdef NOT_DEFINED

    _label.set_font(dflt_font);
    _label.set_background_color({1, 1, 1, 1});
    _label.set_text(U"Hello World!");
    _label.on_click([](const cppgui::Point &pos, int button, int clicks) {

        std::cout << "Label was clicked! (pos = " << pos.x << ", " << pos.y 
            << ", button = " << button << ", clicks = " << clicks << ")" << std::endl;
    });
    _label.set_position({  50, 50 });
    _label.set_extents ({ 200, 50 });
    root_widget()->add_child(&_label);

    _textbox.set_font(dflt_font);
    _textbox.set_position({50, 120});
    _textbox.set_extents({ 200, 30 });
    _textbox.set_text(U"Abc1234567890");

    _stringlist.set_font(dflt_font);
    //_stringlist.set_position({ 50, 240 });
    //_stringlist.set_extents ({200, 395 });
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
    //root_widget()->add_child(&_stringlist);

    _container1.set_position({  50, 120 });
    _container1.set_extents ({ 200, 400 });
    _container1.add_child(&_textbox);
    _container1.add_child(&_stringlist);
    //_container1.set_layout_type(cppgui::Layout_type::header_content);
    _container1.set_layout_manager<cppgui::layouting<GUI_configuration>::Header_content>();
    root_widget()->add_child(&_container1);

    _button.set_font(dflt_font);
    _button.set_position({ 350, 160 });
    _button.set_extents ({ 200,  30 });
    _button.set_label(U"Click Me!");
    root_widget()->add_child(&_button);

    _button2.set_font(dflt_font);
    _button2.set_label(U"Decorated btn");
    _button2.set_glyph(cppgui::Icon_resources<Default_font::size>::close());
    _button2.set_position({ 350, 195 });
    _button2.set_extents ({ 200,  30 });
    root_widget()->add_child(&_button2);

    _glyph_btn.set_glyph(cppgui::Icon_resources<Default_font::size>::close());
    _glyph_btn.set_position({ 570, 160 });
    _glyph_btn.set_extents ({  30,  30 });
    root_widget()->add_child(&_glyph_btn);

    _checkbox.set_font( dflt_font );
    _checkbox.set_tick_glyph( glyph_font, tick_descr );
    _checkbox.set_position({ 350, 50 });
    _checkbox.set_extents ({ 200, 40 });
    _checkbox.set_label( U"Check me!" );
    root_widget()->add_child(&_checkbox);

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
    _menu.set_layout_manager<cppgui::layouting<GUI_configuration>::Header_content>();
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
    root_widget()->add_child(&_input_dlg);

    _vert_scrollbar.set_position({ 750,  50 });
    _vert_scrollbar.set_extents ({  30, 200 });
    _vert_scrollbar.define_values( 150, 40 );
    _vert_scrollbar.on_position_change([&](cppgui::Position pos) { _scrollbar_pos.change_text( std::to_string(pos) ); });
    root_widget()->add_child(&_vert_scrollbar);

    _scrollbar_pos.set_font(dflt_font);
    _scrollbar_pos.set_position({ 800, 50 });
    _scrollbar_pos.set_extents ({ 100, 30 });
    root_widget()->add_child(&_scrollbar_pos);

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
    _listbox.content_pane()->set_separator({ 1, { 0.5f, 0.5f, 0.5f, 1 }});
    root_widget()->add_child(&_listbox);

    // Container with Vertical_slider's

    _container2.set_position({ 750, 280 });
    _container2.set_extents ({ 300, 400 });
    _slider1.textbox().set_font(dflt_font);
    _slider1.slider().define_range({  50, 120 });
    _slider1.slider().set_value( 50 );
    _container2.add_child(&_slider1);
    _slider2.textbox().set_font(dflt_font);
    _slider2.slider().define_range({  -50, 50 });
    _slider2.slider().set_value( 20 );
    _container2.add_child(&_slider2);
    _slider3.textbox().set_font(dflt_font);
    _slider3.slider().define_range({  0.001f, 0.010f });
    _slider3.slider().set_value( 0.001f );
    _container2.add_child(&_slider3);
    _container2.set_layout_manager<cppgui::layouting<GUI_configuration>::Left_to_right>();
    _container2.layout_manager()->set_spacing(5);
    root_widget()->add_child(&_container2);

    root_widget()->set_background_color({ 0, 0.6f, 0.2f, 1 });
    //root_widget()->set_layout_manager<cppgui::layouting<GUI_configuration>::Nil>();

    //root_widget()->add_child(&_menu);
    ////root_widget()->add_child(&_scrollbox);

    _stringlist.take_focus();
    //root_widget()->set_focus_to(&_menu); // ); // &_textbox);
    //_scrollbox.take_focus();
    //_listbox.take_focus();

    #else

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

    // Right panel

    _right_panel.set_id("RIGHT PANEL");
    _right_panel.set_spacing(5);
    _slider1.set_id("SLIDER1");
    _slider1.textbox().set_font(dflt_font);
    _slider1.slider().define_range({  50, 120 });
    _slider1.slider().set_value( 50 );
    _right_panel.add_element(&_slider1);
    _slider2.textbox().set_font(dflt_font);
    _slider2.slider().define_range({  -50, 50 });
    _slider2.slider().set_value( 20 );
    _right_panel.add_element(&_slider2);
    _slider3.textbox().set_font(dflt_font);
    _slider3.slider().define_range({  0.001f, 0.010f });
    _slider3.slider().set_value( 0.001f );
    _right_panel.add_element(&_slider3);

    // Root widget

    _root_widget.set_background_color({ 0, 0.6f, 0.2f, 1 });
    _root_widget.set_left (&_left_panel , {1, 4});
    _root_widget.set_right(&_right_panel, {1, 4});

    #endif

    _root_widget.on_invalidated([this]() { invalidate(); });

    _root_widget.init_layout();     // layout() and init() will be called upon by the Window
                                    // root_widget()->layout();
                                    // root_widget()->init();

    this->init_window(); // will initialize the GUI and must therefore come last here
}

Test_window::Slider_with_display::Slider_with_display()
{
    _slider.on_value_changed([this](const Value &value)
    {
        _textbox.change_text( std::to_string(value) );
    });
    _textbox.on_done([this](const std::u32string &text)
    {
        _slider.change_value( std::stof(cppgui::utf32_to_utf8(text) ) );
    });

    this->set_spacing(5);

    add_element( &_textbox );
    add_element( &_slider );

    //this->set_layout_manager<cppgui::layouting<GUI_config>::Header_content>();
    //layout_manager()->set_spacing(3);
}

void Test_window::Slider_with_display::compute_view_from_data()
{
    Parent_t::compute_view_from_data();

    _textbox.set_text( std::to_string( _slider.value() ) );
}
