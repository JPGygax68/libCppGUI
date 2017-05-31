#include <cppgui/UI_element.hpp>


namespace cppgui
{
    
    void UI_element::move_by(const Point &d)
    {
        _bounds.orig += d;

        // NOTE: this does not invalidate - must be done by caller!
    }

    void UI_element::shift_by(const Point &d)
    {
        // TODO: this can be optimized using renderer/platform specific features ("2d acceleration", textures, etc.)
        move_by(d);
        invalidate();
    }

    void UI_element::set_visible(bool visible)
    {
        _visible = visible;
    }

    void UI_element::mouse_enter()
    {
        //std::cout << "UI_element::mouse_enter()" << std::endl;
        _hovered = true;
        invalidate();
    }

    void UI_element::mouse_exit()
    {
        //std::cout << "UI_element::mouse_exit()" << std::endl;
        _hovered = false;
        invalidate();
    }


    void UI_element::shift_horizontally(Position_delta delta)
    {
        // TODO: 
        //set_position({ position().x + delta, position().y });
        _bounds.orig.x += delta;
        invalidate();
    }

    void UI_element::shift_vertically(Position_delta delta)
    {
        // TODO: 
        //set_position({ position().x, position().y + delta });
        _bounds.orig.y += delta;
        invalidate();
    }

    void UI_element::shift_up(Length length)
    {
        shift_vertically( - static_cast<Position_delta>(length) );
    }

    void UI_element::shift_down(Length length)
    {
        shift_vertically( static_cast<Position_delta>(length) );
    }

} // ns cppgui