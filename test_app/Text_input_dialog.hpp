#pragma once

#include <string>

#include <cppgui/basic_types.hpp>
#include <cppgui/all_widgets.hpp>
/*
#include <cppgui/Container.hpp>
#include <cppgui/Label.hpp>
#include <cppgui/Textbox.hpp>
*/

template <class Config, bool With_layout>
class Text_input_dialog: public cppgui::Container_base<Config, With_layout>
{
public:
    using Parent_t = cppgui::Container_base<Config, With_layout>;
    using Canvas_t = typename cppgui::Widget<Config, With_layout>::Canvas_t;

    Text_input_dialog();

    void set_default_font(const cppgui::Rasterized_font *);

    void set_caption_font(const cppgui::Rasterized_font *);

    void set_caption(const std::u32string &);
    
    void set_prompt(const std::u32string &);

    void render(Canvas_t *, const cppgui::Point &offset) override;

private:
    CPPGUI_DEFINE_WIDGET_TYPES(Config, With_layout);

    //using Label_t = cppgui::Label<Config, With_layout>;
    //using Container_t = cppgui::Container<Config, With_layout>;
    //using Textbox_t = cppgui::Textbox<Config, With_layout>;

    cppgui::Border                  _border = {0};
    cppgui::Rasterized_font         _glyph_font;
    Container_base                  _header_bar;
    Label                           _caption_label;
    //Button                          _close_btn;
    Glyph_button                    _close_btn;
    Container_base                  _main_body;
    Label                           _prompt_label;
    Container_base                  _input_row;
    Textbox                         _textbox;
    Button                          _ok_btn;
};

