#include <iostream>

#include "./Window.ipp"
#include "./cppgui/Label.hpp"

#include "./Test_window.hpp"

#include "./cppgui/Widget.ipp"
#include "./cppgui/Label.ipp"
#include "./cppgui/Container.ipp"
#include "./cppgui/Frame.ipp"

#include <gpc/gl/wrappers.hpp>

#include "./Fonts.hpp"

Test_window::Test_window():
    Window<Test_window>("Test window")
{
    _label.set_font(&Fonts::default_font());
    _label.set_text(U"Hello World!");

    _frame.set_widget(&_label);
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
    delete _renderer;
}

void Test_window::redraw()
{
    std::cout << "Test_window::redraw()" << std::endl;

    if (!_gfxres_ok)
    {
        _label.update_resources(_renderer);
        _gfxres_ok = true;
    }

    _renderer->enter_context();
    _frame.render(_renderer);
    _renderer->leave_context();
    SDL_GL_SwapWindow(sdl_pointer());
}

void Test_window::size_changed(int w, int h)
{
    _label.set_position({ 50, 50 });
    _label.set_extents({ 200, 60 });
    _label.layout();
    _renderer->define_viewport(0, 0, w, h);
}

void Test_window::mouse_motion(int x, int y)
{
    _frame.mouse_motion({x, y});
}
