#pragma once

#include <string>
#include <memory>

struct SDL_Window;
struct SDL_WindowEvent;

class Window {
    struct Deleter;

public:
    using Pointer = std::unique_ptr<SDL_Window, Deleter>;

    static Uint32 redraw_event_id;

    static auto create(const std::string &title = "SDL Window") -> std::unique_ptr<Window>;

    static void dispatch_event(SDL_WindowEvent &ev);
    static void dispatch_redraw(Uint32 win_id);

    ~Window();

    void handle_event(SDL_WindowEvent &ev);

    void request_redraw();


private:
    Window(SDL_Window *);

    struct Deleter { void operator() (SDL_Window *win); };
    class Static_init;

    Pointer _win;
};
