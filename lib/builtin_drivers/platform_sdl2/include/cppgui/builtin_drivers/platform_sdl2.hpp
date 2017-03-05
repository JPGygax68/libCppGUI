#pragma once

// TODO: split into several modules


#include <SDL2/SDL.h>


namespace cppgui {

    using Keycode = SDL_Keysym;
    using Cursor_handle = SDL_Cursor *;

    inline bool is_left     (const Keycode &key) { return key.sym == SDLK_LEFT     ; }
    inline bool is_right    (const Keycode &key) { return key.sym == SDLK_RIGHT    ; }
    inline bool is_up       (const Keycode &key) { return key.sym == SDLK_UP       ; }
    inline bool is_down     (const Keycode &key) { return key.sym == SDLK_DOWN     ; }
    inline bool is_page_up  (const Keycode &key) { return key.sym == SDLK_PAGEUP   ; }
    inline bool is_page_down(const Keycode &key) { return key.sym == SDLK_PAGEDOWN ; }
    inline bool is_backspace(const Keycode &key) { return key.sym == SDLK_BACKSPACE; }
    inline bool is_delete   (const Keycode &key) { return key.sym == SDLK_DELETE   ; }
    inline bool is_home     (const Keycode &key) { return key.sym == SDLK_HOME     ; }
    inline bool is_end      (const Keycode &key) { return key.sym == SDLK_END      ; }
    inline bool is_tab      (const Keycode &key) { return key.sym == SDLK_TAB      ; }
    inline bool is_return   (const Keycode &key) { return key.sym == SDLK_RETURN   ; } // TODO: OR with SDLK_RETURN2 ?
    inline bool is_escape   (const Keycode &key) { return key.sym == SDLK_ESCAPE   ; }

    // Modifiers 

    inline bool is_shift_down() { return (SDL_GetModState() & KMOD_SHIFT) != 0; }
    inline bool is_ctrl_down () { return (SDL_GetModState() & KMOD_CTRL ) != 0; }

    inline bool is_mouse_button_down(int button) { return (SDL_GetMouseState(nullptr, nullptr) & SDL_BUTTON(button)) != 0; }

    inline auto get_current_cursor() -> SDL_Cursor * { return SDL_GetCursor(); }

    inline auto get_arrow_cursor() -> SDL_Cursor* { return SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW); }

    inline auto get_ibeam_cursor() -> SDL_Cursor* { return SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_IBEAM); };

    inline void free_cursor(Cursor_handle cursor) { SDL_FreeCursor(cursor); }

    inline void set_cursor(Cursor_handle cursor) { SDL_SetCursor(cursor); }

} // ns cppgui

#include "./SDL2_window.hpp"