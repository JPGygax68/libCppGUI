#include <iostream>
#include <cppgui/baked_fonts.hpp>
//#include <cppgui/Default_font.hpp>
//#include <cppgui/all_widgets.ipp>
//#include <cppgui/sdl/Window.ipp>
//#include <cppgui/GUI_window.ipp>
//#include "./Fonts.hpp"
#include "./Test_window.hpp"
#include <cppgui/Icon_resources.hpp>
#include <cppgui/unicode.hpp>
#include <cppgui/layouting.hpp>


// Class implementation  --------------------------------------------

Test_window::Test_window():
    cppgui::Window{"Test window"}
{
}

void Test_window::create_ui()
{
    using namespace std::string_literals;

    auto dflt_font = &cppgui::baked_fonts::default_font();

    // TODO: 
    //auto tick_font_data = cppgui::Icon_resources<Default_font::size>::tick_font_data();
    //glyph_font = gpc::fonts::load(tick_font_data.first, tick_font_data.second);
    //auto tick_descr = cppgui::Icon_resources<Default_font::size>::tick_descr();

    root_widget().set_background_color({0, 0.5, 1, 1});

    _label.set_font(dflt_font);
    //_label.set_color({1, 1, 1, 1});
    //_label.set_background_color({1, 1, 1, 1});
    _label.set_text(U"Hello World!");
    _label.on_click([](const cppgui::Point &pos, int button, int clicks) {
        std::cout << "Label was clicked! (pos = " << pos.x << ", " << pos.y 
            << ", button = " << button << ", clicks = " << clicks << ")" << std::endl;
        return true;
    });
    root_widget().add_child(&_label);

    _textbox.set_font(dflt_font);
    _textbox.set_text(U"Abc1234567890");
    root_widget().add_child(&_textbox);

    _button.set_font(dflt_font);
    _button.set_label(U"Click Me!");
    //_button.set_alignment(cppgui::cultural_minor_start);
    root_widget().add_child(&_button);

    _checkbox.set_font(dflt_font);
    //_checkbox.set_tick_glyph(&cppgui::baked_fonts::default_symbol_font(), tick_descr);
    _checkbox.set_label(U"Check me!");
    root_widget().add_child(&_checkbox);

    _glyphbutton.set_font(dflt_font);
    _glyphbutton.set_label(U"Click me too!");
    _glyphbutton.set_glyph(cppgui::Icon_resources::bell());
    root_widget().add_child(&_glyphbutton);

    _vscrollbar.define_sizes(100, 20);
    root_widget().add_child(&_vscrollbar);

    _button_list.resize(15);
    for (auto i = 0U; i < _button_list.size(); i ++)
    {
        _button_list[i].set_font(dflt_font);
        _button_list[i].set_label(cppgui::utf8_to_utf32("Button #"s + std::to_string(i)));
        _button_list[i].set_glyph(cppgui::Icon_resources::bell());
        _listbox.add_item(&_button_list[i]);
    }
    root_widget().add_child(&_listbox);

    _popupbtn.set_font(dflt_font);
    _popupbtn.set_label(U"Popup");
    //_button.set_alignment(cppgui::cultural_minor_start);
    root_widget().add_child(&_popupbtn);


#ifdef NOT_DEFINED

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

    _align_left_ref  .set_font(dflt_font);
    _align_left_ref  .set_label(U"Align left");
    _align_left_tgt  .set_font(dflt_font);
    _align_left_tgt  .set_label(U"LEFT");
    _align_origin_ref.set_font(dflt_font);
    _align_origin_ref.set_label(U"Align origin");
    _align_origin_tgt.set_font(dflt_font);
    _align_origin_tgt.set_label(U"ORIGIN");
    _align_center_ref.set_font(dflt_font);
    _align_center_ref.set_label(U"Align center");
    _align_center_tgt.set_font(dflt_font);
    _align_center_tgt.set_label(U"CENTER");
    _align_right_ref .set_font(dflt_font);
    _align_right_ref .set_label(U"Align right");
    _align_right_tgt .set_font(dflt_font);
    _align_right_tgt .set_label(U"RIGHT");
    _align_top_ref   .set_font(dflt_font);
    _align_top_ref   .set_label(U"Align top");
    _align_top_tgt   .set_font(dflt_font);
    _align_top_tgt   .set_label(U"TOP");
    _align_middle_ref.set_font(dflt_font);
    _align_middle_ref.set_label(U"Align middle");
    _align_middle_tgt.set_font(dflt_font);
    _align_middle_tgt.set_label(U"MIDDLE");
    _align_bottom_ref.set_font(dflt_font);
    _align_bottom_ref.set_label(U"Align bottom");
    _align_bottom_tgt.set_font(dflt_font);
    _align_bottom_tgt.set_label(U"BOTTOM");
    _align_left_ref  .on_pushed([this]() { align_widgets_left  (_align_left_ref  , _align_left_tgt  ); return true; });
    _align_origin_ref.on_pushed([this]() { align_widgets_origin(_align_origin_ref, _align_origin_tgt); return true; });
    _align_center_ref.on_pushed([this]() { align_widgets_center(_align_center_ref, _align_center_tgt); return true; });
    _align_right_ref .on_pushed([this]() { align_widgets_right (_align_right_ref , _align_right_tgt ); return true; });
    root_widget().add_child(&_align_left_ref  ); root_widget().add_child(&_align_left_tgt  );
    root_widget().add_child(&_align_origin_ref); root_widget().add_child(&_align_origin_tgt);
    root_widget().add_child(&_align_center_ref); root_widget().add_child(&_align_center_tgt);
    root_widget().add_child(&_align_right_ref ); root_widget().add_child(&_align_right_tgt );
    root_widget().add_child(&_align_top_ref   ); root_widget().add_child(&_align_top_tgt   );
    root_widget().add_child(&_align_middle_ref); root_widget().add_child(&_align_middle_tgt);
    root_widget().add_child(&_align_bottom_ref); root_widget().add_child(&_align_bottom_tgt);


    //root_widget().switch_focused_child(&_listbox);
    root_widget().set_initial_focus(&_listbox);
}

void Test_window::adjust_layout()
{
    _label      .set_bounds({  50,  50 }, _label      .get_minimal_bounds());
    _textbox    .set_bounds({ 150,  50 }, _textbox    .get_minimal_bounds());
    _button     .set_bounds({ 300,  50 }, _button     .get_minimal_bounds());
    _checkbox   .set_bounds({ 450,  50 }, _checkbox   .get_minimal_bounds());
    _glyphbutton.set_bounds({  50, 100 }, _glyphbutton.get_minimal_bounds());
    _popupbtn   .set_bounds({ 350, 100 }, _popupbtn   .get_minimal_bounds());
    _vscrollbar .set_bounds({  50, 150 }, _vscrollbar .get_minimal_bounds());

    {
        auto bbox = _listbox.get_minimal_bounds();
        bbox.y_min = -120;
        _listbox    .set_bounds({ 150, 150 }, bbox);
    }

    _align_left_ref  .set_bounds({ 600,  50}, _align_left_ref  .get_minimal_bounds());
    _align_left_tgt  .set_bounds({ 650, 100}, _align_left_tgt  .get_minimal_bounds());
    _align_origin_ref.set_bounds({ 750,  50}, _align_origin_ref.get_minimal_bounds());
    _align_origin_tgt.set_bounds({ 800, 100}, _align_origin_tgt.get_minimal_bounds());
    _align_center_ref.set_bounds({ 900,  50}, _align_center_ref.get_minimal_bounds());
    _align_center_tgt.set_bounds({ 950, 100}, _align_center_tgt.get_minimal_bounds());
    _align_right_ref .set_bounds({1050,  50}, _align_right_ref .get_minimal_bounds());
    _align_right_tgt .set_bounds({1100, 100}, _align_right_tgt .get_minimal_bounds());
}

void Test_window::before_draw_ui(void *context)
{
    // TODO: in the future, this may be better done by choosign a correctly customized root widget
    //c->clear({1, 1, 0.8f, 1});
}

void Test_window::align_widgets_left(Widget &ref, Widget &tgt)
{
    tgt.bounds().align_left_to(ref.bounds());
}

void Test_window::align_widgets_origin(Widget &ref, Widget &tgt)
{
    tgt.bounds().align_origin_to(ref.bounds());
}

void Test_window::align_widgets_center(Widget &ref, Widget &tgt)
{
    tgt.bounds().align_center_to(ref.bounds());
}

void Test_window::align_widgets_right(Widget &ref, Widget &tgt)
{
    tgt.bounds().align_right_to(ref.bounds());
}
