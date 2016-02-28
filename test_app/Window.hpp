#pragma once

#include <string>
#include <memory>
#include <map>

struct SDL_Window;
struct SDL_WindowEvent;

/*  We use CRTP ("curiously recurring template pattern") to inject specialization
    into the template.

    Impl must provide the following members:

    void init()
        Called when window is being opened. Use this to bind to the graphics subsystem.
    void redraw()
        Called to draw the contents of the window.
 */
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

protected:
    auto sdl_pointer() -> SDL_Window *;

private:
    struct Deleter { void operator() (SDL_Window *win); };
    class Static_init;

    static auto window_map() -> std::map<uint32_t, Window*> &;

    void handle_event(SDL_WindowEvent &ev);

    Pointer _win;
};
