#include <iostream>

#include "./Window.ipp"

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
    GL(ClearColor, 0, 0.5f, 1, 1);
}

void Test_window::redraw()
{
    std::cout << "Test_window::redraw()" << std::endl;
    GL(Clear, GL_COLOR_BUFFER_BIT);
    SDL_GL_SwapWindow(sdl_pointer());
}
