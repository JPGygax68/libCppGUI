#include <iostream>

#include "./cppgui/Label.hpp"
#include "./cppgui/Textbox.hpp"
#include "./cppgui/Stack.hpp"

#include "./Test_window.hpp"

#include <cppgui/Full_resource_mapper.ipp>
#include <cppgui/Canvas.ipp>
#include <cppgui/Widget.ipp>
#include <cppgui/Button.ipp>
#include <cppgui/Label.ipp>
#include <cppgui/Textbox.ipp>
#include <cppgui/Container.ipp>
#include <cppgui/Stack.ipp>
#include <cppgui/Root_widget.ipp>

#include <cppgui/sdl/Window.hpp>

#include <gpc/gl/wrappers.hpp>

#include "./Fonts.hpp"

// Explicitly instantiate cppgui::sdl::Window template
#include <cppgui/sdl/Window.ipp>

// Class implementation  --------------------------------------------

Test_window::Test_window():
    cppgui::sdl::Window<Test_window>("Test window"),
    _root_widget{ _canvas }
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

    _root_widget.add_child(&_label);
    _root_widget.add_child(&_textbox);
    _root_widget.add_child(&_button);
    _root_widget.add_child(&_stack);

    _button_list.resize(8);
    for (auto i = 0U; i < _button_list.size(); i ++)
    {
        _button_list[i].set_font(&Fonts::default_font());
        _button_list[i].set_label(std::u32string{U"This is button #"} + char32_t(U'1' + i));
        _stack.add_child(&_button_list[i]);
    }
    _stack.set_position({50, 250});
    _stack.set_extents({200, 207});

    _root_widget.set_focus_to(&_textbox);

    _root_widget.on_invalidated([this]() { invalidate(); });

    _root_widget.init_layout();
    _root_widget.layout();
    _root_widget.init();
}

/** Caution! the following is called from the constructor of Window, i.e. *before* the body
    of the constructor of Test_window (which is derived from Window).
 */
void Test_window::init_graphics()
{
    std::cout << "Test_window::init()" << std::endl;

    SDL_GL_SetAttribute(SDL_GL_SHARE_WITH_CURRENT_CONTEXT, 1);
    auto ctx = create_gl_context();
    if (!ctx) throw std::runtime_error("Test_window::init(): failed to create OpenGL context");
    make_gl_context_current(ctx);
    glbinding::Binding::initialize();
    _canvas = new Canvas_t{};
    _canvas->init();
    GL(ClearColor, 0.0f, 0.5f, 1.0f, 1.0f);
}

void Test_window::cleanup_graphics()
{
    assert(_canvas);
    // TODO: renderer cleanup ?
    //_root_widget.cleanup_render_resources(_canvas);
    delete _canvas;
}

void Test_window::redraw()
{
    std::cout << "Test_window::redraw()" << std::endl;

    /* if (!_gfxres_ok)
    {
        _label.update_render_resources(_canvas);
        _gfxres_ok = true;
    } */

    //GL(Clear, GL_COLOR_BUFFER_BIT);
    _canvas->enter_context();
    _root_widget.render(_canvas, { 0, 0 });
    _canvas->leave_context();
    gl_swap();
}

void Test_window::size_changed(int w, int h)
{
    _root_widget.set_extents({ (unsigned)w, (unsigned)h });
    _label.layout();
    _canvas->define_viewport(0, 0, w, h);
}

void Test_window::mouse_motion(int x, int y)
{
    _root_widget.mouse_motion({x, y});
}

void Test_window::mouse_button(int x, int y, int button, Button_direction dir, int count)
{
    _root_widget.mouse_button({ x, y }, button, dir == down ? cppgui::pressed : cppgui::released);

    if (dir == up)
    {
        _root_widget.mouse_click({ x, y, }, button, count);
    }
}

void Test_window::mouse_wheel(int x, int y)
{
    _root_widget.mouse_wheel({x, y});
}

void Test_window::text_input(const char32_t *text, size_t size)
{
    _root_widget.text_input(text, size);
}

void Test_window::key_down(const SDL_Keysym &key)
{
    _root_widget.key_down(key);
}

void Test_window::closing()
{
    cleanup_graphics();
    //_label.cleanup_render_resources(_canvas);
    //_root_widget.cleanup_render_resources(_canvas);
    // TODO: release all resources of all backends
}
