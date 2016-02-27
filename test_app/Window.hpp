#pragma once

#include <string>
#include <memory>

struct SDL_Window;

class Window {
    struct Deleter;

public:
    using Pointer = std::unique_ptr<SDL_Window, Deleter>;

    static auto create(const std::string &title = "SDL Window") -> Pointer;

private:
    struct Window::Deleter {
        void operator() (SDL_Window *win);
    };

};
