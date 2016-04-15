#pragma once

#include <SDL_keyboard.h>

namespace cppgui {

    namespace sdl {

        struct Keyboard_adapter {

            using Keycode = SDL_Keysym;

            static inline bool is_left     (const Keycode &key) { return key.sym == SDLK_LEFT     ; }
            static inline bool is_right    (const Keycode &key) { return key.sym == SDLK_RIGHT    ; }
            static inline bool is_backspace(const Keycode &key) { return key.sym == SDLK_BACKSPACE; }
            static inline bool is_delete   (const Keycode &key) { return key.sym == SDLK_DELETE   ; }
            static inline bool is_home     (const Keycode &key) { return key.sym == SDLK_HOME     ; }
            static inline bool is_end      (const Keycode &key) { return key.sym == SDLK_END      ; }
            static inline bool is_tab      (const Keycode &key) { return key.sym == SDLK_TAB      ; }

            // Modifiers 

            static inline bool is_shift_down() { return (SDL_GetModState() & KMOD_SHIFT) != 0; }
            static inline bool is_ctrl_down () { return (SDL_GetModState() & KMOD_CTRL ) != 0; }
        };

    } // ns sdl

} // ns cppgui
