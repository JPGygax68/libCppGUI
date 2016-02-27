#include <stdexcept>

#include <SDL.h>

#include "./Window.hpp"

class _Initializer {
public:
    _Initializer() {
        //SDL_Init(SDL_INIT_EVERYTHING); //SDL_INIT_VIDEO);
        if (SDL_VideoInit(nullptr) < 0) throw std::runtime_error("trying to initialize SDL video subsystem");
        atexit(&SDL_VideoQuit);
    }
} _init;

auto Window::create(const std::string& title) -> Pointer
{
    auto win = SDL_CreateWindow(title.c_str(), 
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 0, 0, 
        SDL_WINDOW_MAXIMIZED|SDL_WINDOW_OPENGL);

    return Pointer{ win };
}

void Window::Deleter::operator()(SDL_Window * win)
{
    SDL_DestroyWindow(win);
}
