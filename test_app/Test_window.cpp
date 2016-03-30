#include <iostream>

#include <cppgui/Full_resource_mapper.ipp>
#include <cppgui/Canvas.ipp>
#include <cppgui/Widget.ipp>
#include <cppgui/Button.ipp>
#include <cppgui/Label.ipp>
#include <cppgui/Textbox.ipp>
#include <cppgui/Container.ipp>
#include <cppgui/Stack.ipp>
#include <cppgui/Root_widget.ipp>
#include <cppgui/sdl/Window.ipp>
#include <cppgui/GUI_window.ipp>

#include "./Test_window.hpp"

#include "./Fonts.hpp"

template cppgui::sdl::Window<Test_window>;

// Class implementation  --------------------------------------------

Test_window::Test_window(): Parent("Test window")
{
    _label.set_font(&Fonts::default_font());
    _label.set_text(U"Hello World!");
    _label.on_click([](const cppgui::Position &pos, int button, int clicks) {
        std::cout << "Label was clicked! (pos = " << pos.x << ", " << pos.y 
            << ", button = " << button << ", clicks = " << clicks << ")" << std::endl;
    });
    _label.set_position({ 50, 50 });
    //_label.set_extents({ 0, 0 }); //200, 50 });

    _textbox.set_font(&Fonts::default_font());
    _textbox.set_position({50, 120});
    //_textbox.set_extents({ 200, 30 });
    _textbox.set_text(U"Abc1234567890");
    
    _button.set_font(&Fonts::default_font());
    _button.set_position({50, 160});
    _button.set_extents({200, 30});
    _button.set_label(U"Click Me!");

    root_widget().add_child(&_label);
    root_widget().add_child(&_textbox);
    root_widget().add_child(&_button);
    root_widget().add_child(&_stack);

    _button_list.resize(8);
    for (auto i = 0U; i < _button_list.size(); i ++)
    {
        _button_list[i].set_font(&Fonts::default_font());
        _button_list[i].set_label(std::u32string{U"This is button #"} + char32_t(U'1' + i));
        _stack.add_child(&_button_list[i]);
    }
    _stack.set_position({50, 250});
    _stack.set_extents({200, 207});

    root_widget().set_focus_to(&_textbox);

    root_widget().on_invalidated([this]() { invalidate(); });

    root_widget().init_layout();
    root_widget().layout();
    root_widget().init();
}

