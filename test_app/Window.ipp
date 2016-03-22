#pragma once

#include <locale>
#include <codecvt>

#include "./Window.hpp"

#include <stdexcept>
#include <iostream>
#include <map>

#include <SDL.h>
#include <SDL_events.h>

#include "./Window.hpp"

static class Static_init {
public:
    Static_init()
    {
        //SDL_Init(SDL_INIT_EVERYTHING); //SDL_INIT_VIDEO);
        if (SDL_VideoInit(nullptr) < 0) throw std::runtime_error("trying to initialize SDL video subsystem");
        atexit(&SDL_VideoQuit);
    }
} _init;

template <class Impl>
Window<Impl>::Window(const std::string &title, int w, int h)
{
    auto win = SDL_CreateWindow(title.c_str(),
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w, h,
        SDL_WINDOW_RESIZABLE | SDL_WINDOW_MAXIMIZED | SDL_WINDOW_OPENGL);

    _win.reset(win);

    window_map()[SDL_GetWindowID(win)] = this;

    static_cast<Impl*>(this)->init_graphics();
}

template <class Impl>
Window<Impl>::~Window()
{
    window_map().erase(SDL_GetWindowID(_win.get()));
}

template <class Impl>
auto Window<Impl>::redraw_event_id() -> uint32_t &
{
    static uint32_t id = SDL_RegisterEvents(1);
    return id;
}

template <class Impl>
auto Window<Impl>::window_map() -> std::map<uint32_t, Window*> &
{
    static std::map<uint32_t, Window*> map;

    return map;
}

template <class Impl>
void Window<Impl>::dispatch_window_event(SDL_WindowEvent &ev)
{
    window_map()[ev.windowID]->handle_window_event(ev);
}

template<class Impl>
void Window<Impl>::dispatch_mousemotion_event(SDL_MouseMotionEvent & ev)
{
    window_map()[ev.windowID]->handle_mousemotion_event(ev);
}

template<class Impl>
void Window<Impl>::dispatch_mousebutton_event(SDL_MouseButtonEvent & ev)
{
    window_map()[ev.windowID]->handle_mousebutton_event(ev);
}

template<class Impl>
void Window<Impl>::dispatch_textinput_event(SDL_TextInputEvent & ev)
{
    window_map()[ev.windowID]->handle_textinput_event(ev);
}

template<class Impl>
void Window<Impl>::dispatch_keydown_event(SDL_KeyboardEvent & ev)
{
    window_map()[ev.windowID]->handle_keydown_event(ev);
}

template <class Impl>
void Window<Impl>::dispatch_redraw(uint32_t win_id)
{
    static_cast<Impl*>(window_map()[win_id])->redraw();
}

template<class Impl>
auto Window<Impl>::sdl_pointer() -> SDL_Window *
{
    return _win.get();
}

template <class Impl>
void Window<Impl>::handle_window_event(SDL_WindowEvent &ev)
{
    std::cout << "Window::handle_window_event()" << std::endl;

    switch (ev.event)
    {
    case SDL_WINDOWEVENT_SHOWN:
        request_redraw();
        break;
    case SDL_WINDOWEVENT_MAXIMIZED:
        request_redraw();
        break;
    case SDL_WINDOWEVENT_SIZE_CHANGED:
        static_cast<Impl*>(this)->size_changed(ev.data1, ev.data2); // width & height, respectively
        request_redraw();
        break;
    case SDL_WINDOWEVENT_CLOSE:
        static_cast<Impl*>(this)->closing();
        break;
    }
}


template <class Impl>
void Window<Impl>::handle_mousemotion_event(SDL_MouseMotionEvent &ev)
{
    // std::cout << "Window::handle_mousemotion_event()" << std::endl;

    static_cast<Impl*>(this)->mouse_motion(ev.x, ev.y);
}

template<class Impl>
void Window<Impl>::handle_mousebutton_event(SDL_MouseButtonEvent & ev)
{
    auto dir = ev.state == SDL_PRESSED ? down : up;
    static_cast<Impl*>(this)->mouse_button(ev.x, ev.y, ev.button, dir, ev.clicks);
}

template<class Impl>
void Window<Impl>::handle_textinput_event(SDL_TextInputEvent & ev)
{
#if _MSC_VER == 1900
    std::wstring_convert<std::codecvt_utf8<int32_t>, int32_t> utf32conv;
    auto utf32 = utf32conv.from_bytes(ev.text);
    static_cast<Impl*>(this)->text_input(reinterpret_cast<const char32_t *>(utf32.data()), utf32.size());
#else
    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> utf32conv;
    auto utf32 = utf32conv.from_bytes(ev.text);
    _text_input_handler(utf32.c_str(), utf32.size());
    static_cast<Impl*>(this)->text_input(utf32.data(), utf32.size());
#endif
}

template<class Impl>
void Window<Impl>::handle_keydown_event(SDL_KeyboardEvent &ev)
{
    static_cast<Impl*>(this)->key_down(ev.keysym); // TODO: pass "repeat" too ?
}

template <class Impl>
void Window<Impl>::request_redraw()
{
    std::cout << "Window::request_redraw()" << std::endl;

    SDL_Event ev;
    ev.type = redraw_event_id();
    SDL_UserEvent &ue = ev.user;
    ue.windowID = SDL_GetWindowID(_win.get());
    SDL_PushEvent(&ev);
}

template <class Impl>
void Window<Impl>::Deleter::operator()(SDL_Window * win)
{
    SDL_DestroyWindow(win);
}
