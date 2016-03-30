#pragma once

#include <SDL_mouse.h> // TODO: move to .ipp file ?

namespace cppgui {

    namespace sdl {
    
        struct Mouse_adapter {

            using Cursor_handle = SDL_Cursor *;

            static inline bool is_button_down(int button) { return (SDL_GetMouseState(nullptr, nullptr) & SDL_BUTTON(button)) != 0; }

            static auto get_current_cursor() {

                return SDL_GetCursor();
            }

            static auto get_arrow_cursor() {

                // TODO: do we have to manage this resource ?
                return SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);
            }

            static auto get_ibeam_cursor() {

                // TODO: do we have to manage this resource ?
                return SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_IBEAM);
            }

            static void free_cursor(Cursor_handle cursor) {

                SDL_FreeCursor(cursor);
            }

            static void set_cursor(Cursor_handle cursor) {

                SDL_SetCursor(cursor);
            }

        };

    } // ns sdl

} // ns cppgui