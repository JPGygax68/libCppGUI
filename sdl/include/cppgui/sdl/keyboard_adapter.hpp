#pragma once

#include <SDL_keyboard.h>

namespace cppgui {

    namespace sdl {

        struct Keyboard_adapter {

            using Keycode = SDL_Keysym;

            static inline bool is_left     (const Keycode &key) { return key.sym == SDLK_LEFT     ; }
            static inline bool is_right    (const Keycode &key) { return key.sym == SDLK_RIGHT    ; }
            static inline bool is_up       (const Keycode &key) { return key.sym == SDLK_UP       ; }
            static inline bool is_down     (const Keycode &key) { return key.sym == SDLK_DOWN     ; }
            static inline bool is_page_up  (const Keycode &key) { return key.sym == SDLK_PAGEUP   ; }
            static inline bool is_page_down(const Keycode &key) { return key.sym == SDLK_PAGEDOWN ; }
            static inline bool is_backspace(const Keycode &key) { return key.sym == SDLK_BACKSPACE; }
            static inline bool is_delete   (const Keycode &key) { return key.sym == SDLK_DELETE   ; }
            static inline bool is_home     (const Keycode &key) { return key.sym == SDLK_HOME     ; }
            static inline bool is_end      (const Keycode &key) { return key.sym == SDLK_END      ; }
            static inline bool is_tab      (const Keycode &key) { return key.sym == SDLK_TAB      ; }
            static inline bool is_return   (const Keycode &key) { return key.sym == SDLK_RETURN   ; } // TODO: OR with SDLK_RETURN2 ?
            static inline bool is_escape   (const Keycode &key) { return key.sym == SDLK_ESCAPE   ; }

            // Modifiers 

            static inline bool is_shift_down() { return (SDL_GetModState() & KMOD_SHIFT) != 0; }
            static inline bool is_ctrl_down () { return (SDL_GetModState() & KMOD_CTRL ) != 0; }
        };

    } // ns sdl

} // ns cppgui