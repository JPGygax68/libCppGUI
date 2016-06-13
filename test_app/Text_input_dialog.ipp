#include "./Text_input_dialog.hpp"

template<class Config, bool With_layout>
Text_input_dialog<Config, With_layout>::Text_input_dialog()
{
    auto font_data = cppgui::Icon_resources<24>::glyph_font_data();
    _glyph_font = gpc::fonts::load(font_data.first, font_data.second);

    //set_layout_type(cppgui::Layout_type::header_content);
    set_layout_manager<layouting::Header_content>();
    set_background_color(default_dialog_background_color());
    _border = {1, {0, 0, 0, 1}};
    //set_padding(1);

    //_header_bar.set_layout_type(cppgui::Layout_type::content_tail);
    _header_bar.set_layout_manager<layouting::Content_tail>();
    _caption_label.set_background_color({0.8f, 0.8f, 0.8f, 1});
    //_caption_label.set_padding({5, 3, 5, 3});
    // TODO: mechanism to choose between available sizes dynamically ?
    _close_btn.set_glyph(cppgui::Icon_resources<16>::close());
    //_close_btn.enable_border(false);
    //_close_btn.set_border({0});
    _header_bar.add_child(&_caption_label);
    _header_bar.add_child(&_close_btn);

    _prompt_label.set_minor_alignment(cppgui::Alignment::cultural_minor_start);
    _prompt_label.set_major_alignment(cppgui::Alignment::cultural_major_middle);
    //_prompt_label.set_padding(0); // {3, 3, 3, 3});

    //_input_row.set_layout_type(cppgui::Layout_type::content_tail);
    _input_row.set_layout_manager<layouting::Content_tail>();
    //_input_row.set_padding(0);
    _input_row.layout_manager()->set_spacing(5);
    //_textbox.set_padding(3);
    _input_row.add_child(&_textbox);
    _ok_btn.set_label(U"Ok");
    _input_row.add_child(&_ok_btn);

    //_main_body.set_layout_type(cppgui::Layout_type::content_footer);
    _main_body.set_layout_manager<layouting::Content_footer>();
    //_main_body.set_padding(8);
    _main_body.add_child(&_prompt_label);
    _main_body.add_child(&_input_row);

    add_child(&_header_bar);
    add_child(&_main_body);
}

/*
template<class Config, bool With_layout>
void Text_input_dialog<Config, With_layout>::set_border(const Border &border)
{
    _border = border;
}
*/

template<class Config, bool With_layout>
void Text_input_dialog<Config, With_layout>::set_default_font(const cppgui::Rasterized_font *font)
{
    _prompt_label.set_font(font);
    _textbox.set_font(font);
    _ok_btn.set_font(font);
}

template<class Config, bool With_layout>
void Text_input_dialog<Config, With_layout>::set_caption_font(const cppgui::Rasterized_font *font)
{
    _caption_label.set_font(font);
}

template<class Config, bool With_layout>
void Text_input_dialog<Config, With_layout>::set_caption(const std::u32string &caption)
{
    _caption_label.set_text(caption);
}

template<class Config, bool With_layout>
void Text_input_dialog<Config, With_layout>::set_prompt(const std::u32string &text)
{
    _prompt_label.set_text(text);
}

template<class Config, bool With_layout>
void Text_input_dialog<Config, With_layout>::render(Canvas_t *canvas, const cppgui::Point &offset)
{
    Parent_t::render(canvas, offset);

    draw_borders(canvas, offset, _border.width, _border.color);
}
