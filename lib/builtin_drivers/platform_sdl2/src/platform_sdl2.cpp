#include <cppgui/builtin_drivers/platform_sdl2.hpp>

namespace cppgui
{
    namespace sdl2
    {
        auto get_current_cursor() -> SDL_Cursor *
        {
            return SDL_GetCursor();
        }

        auto get_arrow_cursor() -> SDL_Cursor*    
        {
            // TODO: do we have to manage this resource ?
            return SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);
        }

        auto get_ibeam_cursor() -> SDL_Cursor*

        {
            // TODO: do we have to manage this resource ?
            return SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_IBEAM);
        }

        void free_cursor(Cursor_handle cursor)
        {
            SDL_FreeCursor(cursor);
        }

        void set_cursor(Cursor_handle cursor)
        {
            SDL_SetCursor(cursor);
        }

    } // ns sdl2

} // ns cppgui

