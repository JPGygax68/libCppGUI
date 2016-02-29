#include <iostream>

#include "./Window.ipp"
#include "./cppgui/Root_widget.hpp"
#include "./cppgui/Root_widget.ipp"

#include "./Test_window.hpp"

#include <gpc/gl/wrappers.hpp>

Test_window::Test_window():
    Window<Test_window>("Test window")
{
}

void Test_window::init()
{
    std::cout << "Test_window::init()" << std::endl;

    SDL_GL_SetAttribute(SDL_GL_SHARE_WITH_CURRENT_CONTEXT, 1);
    auto ctx = SDL_GL_CreateContext(sdl_pointer());
    if (!ctx) throw std::runtime_error("Test_window::init(): failed to create OpenGL context");
    SDL_GL_MakeCurrent(sdl_pointer(), ctx);
    glbinding::Binding::initialize();
    _renderer = new Renderer{};
    GL(ClearColor, 0.0f, 0.5f, 1.0f, 1.0f);
}

void Test_window::cleanup()
{
    assert(_renderer);
    // TODO: renderer cleanup ?
    delete _renderer;
}

void Test_window::redraw()
{
    std::cout << "Test_window::redraw()" << std::endl;
    //GL(Clear, GL_COLOR_BUFFER_BIT);
    _renderer->enter_context();
    _root_widget.render(_renderer);
    _renderer->leave_context();
    SDL_GL_SwapWindow(sdl_pointer());
}

void Test_window::size_changed(int w, int h)
{
    _root_widget.set_size(w, h);
}
