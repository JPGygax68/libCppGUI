#include "./Text_input_dialog.hpp"

template<class Config, bool With_layout>
Text_input_dialog<Config, With_layout>::Text_input_dialog()
{
    set_layout_type(cppgui::Layout_type::header_content);

    _caption_label.set_background_color({0.8f, 0.8f, 0.8f, 1});
    _caption_label.set_padding({5, 3, 5, 3});

    _prompt_label.set_horizontal_alignment(cppgui::Alignment::left);
    _prompt_label.set_vertical_alignment(cppgui::Alignment::top);
    _prompt_label.set_padding({3, 3, 3, 3});
    _textbox.set_padding({3, 3, 3, 3});

    _main_body.set_layout_type(cppgui::Layout_type::content_footer);
    _main_body.set_padding({3, 3, 3, 3});
    _main_body.add_child(&_prompt_label);
    _main_body.add_child(&_textbox);

    add_child(&_caption_label);
    add_child(&_main_body);
}

template<class Config, bool With_layout>
void Text_input_dialog<Config, With_layout>::set_default_font(const cppgui::Rasterized_font *font)
{
    _prompt_label.set_font(font);
    _textbox.set_font(font);
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
