#pragma once

#include <string>

#include <cppgui/all_widgets.hpp>
/*
#include <cppgui/Container.hpp>
#include <cppgui/Label.hpp>
#include <cppgui/Textbox.hpp>
*/

template <class Config, bool With_layout>
class Text_input_dialog: public cppgui::Container<Config, With_layout>
{
public:

    Text_input_dialog();

    void set_default_font(const cppgui::Rasterized_font *);

    void set_caption_font(const cppgui::Rasterized_font *);

    void set_caption(const std::u32string &);
    
    void set_prompt(const std::u32string &);

private:
    CPPGUI_DEFINE_WIDGET_TYPES(Config, With_layout);

    //using Label_t = cppgui::Label<Config, With_layout>;
    //using Container_t = cppgui::Container<Config, With_layout>;
    //using Textbox_t = cppgui::Textbox<Config, With_layout>;

    cppgui::Rasterized_font         _glyph_font;
    Container                       _header_bar;
    Label                           _caption_label;
    Button                          _close_btn;
    Container                       _main_body;
    Label                           _prompt_label;
    Textbox                         _textbox;
};

