#pragma once

#include <string>
#include <memory>
#include <map>

struct SDL_Window;
struct SDL_WindowEvent;
struct SDL_MouseMotionEvent;

/*  We use CRTP ("curiously recurring template pattern") to inject specialization
    into the template.

    Impl must provide the following members:

    void init_graphics()
        Called when window is being opened, which is upon construction, BEFORE
        the derived constructor is called! Use this to bind to the graphics subsystem.
    void size_changed(int w, int h)
        Called when the size of the window changes (includes window creation).
        TODO: use Size struct instead of w, h
    void mouse_motion(const Position &)
        Called when the user is moving the mouse within the confines of the window.
    void redraw()
        Called to draw the contents of the window.
 */
template <class Impl> class Window {
    struct Deleter;

public:
    using Pointer = std::unique_ptr<SDL_Window, Deleter>;

    static auto redraw_event_id() -> uint32_t &;

    Window(const std::string &title, int w = 800, int h = 600); // TODO: should probably be made protected
    ~Window();

    void request_redraw();

    static void dispatch_window_event(SDL_WindowEvent &ev);
    static void dispatch_mousemotion_event(SDL_MouseMotionEvent &ev);
    static void dispatch_redraw(uint32_t win_id);

protected:
    auto sdl_pointer() -> SDL_Window *;

private:
    struct Deleter { void operator() (SDL_Window *win); };
    class Static_init;

    static auto window_map() -> std::map<uint32_t, Window*> &;

    void handle_window_event(SDL_WindowEvent &ev);
    void handle_mousemotion_event(SDL_MouseMotionEvent &ev);

    Pointer _win;
};
