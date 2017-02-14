#include <cppgui/builtin_drivers/session_sdl2.hpp>

namespace cppgui
{
    auto Mouse_adapter::get_current_cursor() -> SDL_Cursor *
    {
        return SDL_GetCursor();
    }

    auto Mouse_adapter::get_arrow_cursor() -> SDL_Cursor*    
    {
        // TODO: do we have to manage this resource ?
        return SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);
    }

    auto Mouse_adapter::get_ibeam_cursor() -> SDL_Cursor*
    
    {
        // TODO: do we have to manage this resource ?
        return SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_IBEAM);
    }

    void Mouse_adapter::free_cursor(Cursor_handle cursor)
    {
        SDL_FreeCursor(cursor);
    }

    void Mouse_adapter::set_cursor(Cursor_handle cursor)
    {
        SDL_SetCursor(cursor);
    }
}

