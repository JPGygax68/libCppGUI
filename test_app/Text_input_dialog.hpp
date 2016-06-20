#pragma once

#include <string>

#include <cppgui/basic_types.hpp>
#include <cppgui/Default_widgets.hpp>
#include <cppgui/Box_model.hpp>

/*
#include <cppgui/Container.hpp>
#include <cppgui/Label.hpp>
#include <cppgui/Textbox.hpp>
*/

namespace _text_input_dlg
{
    template<int BorderWidth, int PaddingWidth>
    struct Box_model_wrapper
    {
        template<class Parent> using Aspect = cppgui::Fixed_border_and_padding_box_model<BorderWidth, PaddingWidth, Parent>;
    };
}

template <class Config, bool With_layout>
class Text_input_dialog: public cppgui::Container_base<Config, With_layout, _text_input_dlg::Box_model_wrapper<1, 0>::template Aspect>
{
public:
    using Parent_t = cppgui::Container_base<Config, With_layout, _text_input_dlg::Box_model_wrapper<1, 0>::template Aspect>;
    using Canvas_t = typename cppgui::Widget<Config, With_layout>::Canvas_t;

    Text_input_dialog();

    void set_default_font(const cppgui::Rasterized_font *);

    void set_caption_font(const cppgui::Rasterized_font *);

    void set_caption(const std::u32string &);
    
    void set_prompt(const std::u32string &);

    void render(Canvas_t *, const cppgui::Point &offset) override;

private:

    // CPPGUI_DEFINE_WIDGET_TYPES(Config, With_layout);

    //using Label_t = cppgui::Label<Config, With_layout>;
    //using Container_t = cppgui::Container<Config, With_layout>;
    //using Textbox_t = cppgui::Textbox<Config, With_layout>;

    cppgui::Border                              _border = {0};
    cppgui::Rasterized_font                     _glyph_font;
    Container_base                              _header_bar;
    cppgui::Label<Config, With_layout>          _caption_label;
    //Button                                      _close_btn;
    cppgui::Glyph_button<Config, With_layout>   _close_btn;
    Container_base                              _main_body;
    cppgui::Label<Config, With_layout>          _prompt_label;
    Container_base                              _input_row;
    cppgui::Textbox<Config, With_layout>        _textbox;
    cppgui::Button<Config, With_layout>         _ok_btn;
};

