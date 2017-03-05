#pragma once

#include <cstdint>
#include <string>
#include <memory>
#include <map>
#include <functional>

#include "SDL_exception.hpp"

struct SDL_Window;
struct SDL_WindowEvent;
struct SDL_MouseMotionEvent;
struct SDL_MouseButtonEvent;
struct SDL_MouseWheelEvent;
struct SDL_TextInputEvent;
struct SDL_KeyboardEvent;
struct SDL_UserEvent;
struct SDL_Keysym;
using SDL_GLContext = void *;

// TODO: split into base class and specialization for OpenGL

class SDL_window 
{
    struct Deleter;

public:
    using Pointer = std::unique_ptr<SDL_Window, Deleter>;
    struct Extents { int w, h; };
    enum Button_direction { down = 0, up = 1 };

    explicit SDL_window(const std::string &title, int w = 800, int h = 600); // TODO: should probably be made protected
    ~SDL_window();

    auto id() -> uint32_t;

    // OpenGL
    // TODO: move to renderer aspect

    auto get_current_gl_context() -> SDL_GLContext;
    auto create_gl_context() -> SDL_GLContext;           
    static void delete_gl_context(SDL_GLContext);
    void make_gl_context_current(SDL_GLContext);
    void gl_swap();

    void invalidate();

    auto client_extents() const -> Extents;

    //virtual void closing() {};

    static void dispatch_window_event(SDL_WindowEvent &ev);
    static void dispatch_mousemotion_event(SDL_MouseMotionEvent &ev);
    static void dispatch_mousebutton_event(SDL_MouseButtonEvent &ev);
    static void dispatch_mousewheel_event(SDL_MouseWheelEvent &ev);
    static void dispatch_textinput_event(SDL_TextInputEvent &ev);
    static void dispatch_keydown_event(SDL_KeyboardEvent &ev);
    static void dispatch_redraw(uint32_t win_id);
    static void dispatch_custom_event(uint32_t win_id);

    // Interface towards Application<>

    static void for_each_window(std::function<void(SDL_window*)>);

    void end_of_event_burst();

protected:

    void init_window();     // use CRTP-override (note: you MUST call this method from the override!)
    void cleanup_window();  // ditto

    void handle_window_event     (SDL_WindowEvent      &ev);
    void handle_mousemotion_event(SDL_MouseMotionEvent &ev);
    void handle_mousebutton_event(SDL_MouseButtonEvent &ev);
    void handle_mousewheel_event (SDL_MouseWheelEvent  &ev);
    void handle_textinput_event  (SDL_TextInputEvent   &ev);
    void handle_keydown_event    (SDL_KeyboardEvent    &ev);
    void handle_redraw();
    // /*virtual*/ void handle_custom_event(SDL_UserEvent &) {}

    virtual void size_changed(Sint32 data1, Sint32 data2) {}

private:

    SDL_window(const SDL_window &) = delete; // no copy constructor please
    struct Deleter { void operator() (SDL_Window *win); };

    struct Static_init;
    static Static_init _initializer;

    static auto window_map() -> std::map<uint32_t, SDL_window*> &;

    Pointer         _win;
    bool            _must_redraw;
};
