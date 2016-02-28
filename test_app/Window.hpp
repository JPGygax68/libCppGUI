#pragma once

#include <string>
#include <memory>
#include <map>

struct SDL_Window;
struct SDL_WindowEvent;

template <class Impl> class Window {
    struct Deleter;

public:
    using Pointer = std::unique_ptr<SDL_Window, Deleter>;

    static auto redraw_event_id() -> uint32_t &;

    Window(const std::string &title); // TODO: should probably be made protected
    ~Window();

    void request_redraw();

    static void dispatch_event(SDL_WindowEvent &ev);
    static void dispatch_redraw(uint32_t win_id);

private:
    struct Deleter { void operator() (SDL_Window *win); };
    class Static_init;

    static auto window_map() -> std::map<uint32_t, Window*> &;

    void handle_event(SDL_WindowEvent &ev);

    Pointer _win;
};
