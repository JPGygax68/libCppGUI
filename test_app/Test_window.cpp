#include <iostream>

#include "./Window.ipp"
#include "./cppgui/Label.hpp"

#include "./Test_window.hpp"

#include <cppgui/Resource_mapper.ipp>
#include <cppgui/Widget.ipp>
#include <cppgui/Label.ipp>
#include <cppgui/Container.ipp>
#include <cppgui/Root_widget.ipp>

#include <gpc/gl/wrappers.hpp>

#include "./Fonts.hpp"

Test_window::Test_window():
    Window<Test_window>("Test window")
{
    _label.set_font(&Fonts::default_font());
    _label.set_text(U"Hello World!");
    _label.on_click([](const cppgui::Position &pos, int button, int clicks) {
        std::cout << "Label was clicked! (pos = " << pos.x << ", " << pos.y 
            << ", button = " << button << ", clicks = " << clicks << ")" << std::endl;
    });

    _root_widget.add_child(&_label);

    _root_widget.on_invalidated([this]() { request_redraw(); });
}

void Test_window::init_graphics()
{
    std::cout << "Test_window::init()" << std::endl;

    SDL_GL_SetAttribute(SDL_GL_SHARE_WITH_CURRENT_CONTEXT, 1);
    auto ctx = SDL_GL_CreateContext(sdl_pointer());
    if (!ctx) throw std::runtime_error("Test_window::init(): failed to create OpenGL context");
    SDL_GL_MakeCurrent(sdl_pointer(), ctx);
    glbinding::Binding::initialize();
    _renderer = new Renderer{};
    _renderer->init();
    GL(ClearColor, 0.0f, 0.5f, 1.0f, 1.0f);
}

void Test_window::cleanup_graphics()
{
    assert(_renderer);
    // TODO: renderer cleanup ?
    _root_widget.cleanup_render_resources(_renderer);
    delete _renderer;
}

void Test_window::redraw()
{
    std::cout << "Test_window::redraw()" << std::endl;

    if (!_gfxres_ok)
    {
        _label.update_render_resources(_renderer);
        _gfxres_ok = true;
    }

    //GL(Clear, GL_COLOR_BUFFER_BIT);
    _renderer->enter_context();
    //_label.render(_renderer, {0, 0});
    _root_widget.render(_renderer, { 0, 0 });
    _renderer->leave_context();
    SDL_GL_SwapWindow(sdl_pointer());
}

void Test_window::size_changed(int w, int h)
{
    _root_widget.set_extents({ (unsigned)w, (unsigned)h });
    _label.set_position({ 50, 50 });
    _label.set_extents({ 200, 60 });
    _label.layout();
    _renderer->define_viewport(0, 0, w, h);
}

void Test_window::mouse_motion(int x, int y)
{
    _root_widget.mouse_motion({x, y});
}

void Test_window::mouse_button(int x, int y, int button, Button_direction dir, int count)
{
    if (dir == up)
    {
        _root_widget.mouse_click({ x, y, }, button, count);
    }
}

void Test_window::closing()
{
    _label.cleanup_render_resources(_renderer);
    _root_widget.cleanup_render_resources(_renderer);
}
