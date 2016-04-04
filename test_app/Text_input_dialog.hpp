#pragma once

#include <string>

#include <cppgui/Container.hpp>
#include <cppgui/Label.hpp>
#include <cppgui/Textbox.hpp>

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
    using Label_t = cppgui::Label<Config, With_layout>;
    using Container_t = cppgui::Container<Config, With_layout>;
    using Textbox_t = cppgui::Textbox<Config, With_layout>;

    Label_t             _caption_label;
    Container_t         _main_body;
    Label_t             _prompt_label;
    Textbox_t           _textbox;
};

