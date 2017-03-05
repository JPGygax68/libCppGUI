#pragma once

// TODO: split into several modules


#include <SDL2/SDL.h>


namespace cppgui {

    using Keycode = SDL_Keysym;

    namespace sdl2 {

        static bool is_left     (const Keycode &key) { return key.sym == SDLK_LEFT     ; }
        static bool is_right    (const Keycode &key) { return key.sym == SDLK_RIGHT    ; }
        static bool is_up       (const Keycode &key) { return key.sym == SDLK_UP       ; }
        static bool is_down     (const Keycode &key) { return key.sym == SDLK_DOWN     ; }
        static bool is_page_up  (const Keycode &key) { return key.sym == SDLK_PAGEUP   ; }
        static bool is_page_down(const Keycode &key) { return key.sym == SDLK_PAGEDOWN ; }
        static bool is_backspace(const Keycode &key) { return key.sym == SDLK_BACKSPACE; }
        static bool is_delete   (const Keycode &key) { return key.sym == SDLK_DELETE   ; }
        static bool is_home     (const Keycode &key) { return key.sym == SDLK_HOME     ; }
        static bool is_end      (const Keycode &key) { return key.sym == SDLK_END      ; }
        static bool is_tab      (const Keycode &key) { return key.sym == SDLK_TAB      ; }
        static bool is_return   (const Keycode &key) { return key.sym == SDLK_RETURN   ; } // TODO: OR with SDLK_RETURN2 ?
        static bool is_escape   (const Keycode &key) { return key.sym == SDLK_ESCAPE   ; }

        // Modifiers 

        static bool is_shift_down() { return (SDL_GetModState() & KMOD_SHIFT) != 0; }
        static bool is_ctrl_down () { return (SDL_GetModState() & KMOD_CTRL ) != 0; }

        using Cursor_handle = SDL_Cursor *;

        static inline bool is_mouse_button_down(int button) { return (SDL_GetMouseState(nullptr, nullptr) & SDL_BUTTON(button)) != 0; }

        static auto get_current_cursor() -> SDL_Cursor *;

        static auto get_arrow_cursor() -> SDL_Cursor*;

        static auto get_ibeam_cursor() -> SDL_Cursor*;

        static void free_cursor(Cursor_handle cursor);

        static void set_cursor(Cursor_handle cursor);

    } // ns sdl2

} // ns cppgui

#include "./SDL2_window.hpp"