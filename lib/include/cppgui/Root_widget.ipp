#include <cppgui/Root_widget.hpp>

namespace cppgui {

    /*
    template<class Config, bool With_layout>
    Root_widget<Config, With_layout>::Root_widget(Canvas_t *cv)
    {
        _canvas = cv;
    }
    */

    /*
    template<class GUIConfig, bool WithLayout>
    void Root_widget<GUIConfig, WithLayout>::add_child(Widget<GUIConfig, WithLayout> *child)
    {
        _add_child(child);
        child->added_to_container(static_cast<Abstract_container_t*>(this));
    }
    */

    template<class Config, bool With_layout>
    void Root_widget<Config, With_layout>::set_canvas(Canvas_t *cv)
    {
        _canvas = cv;
    }

    template<class Config, bool With_layout>
    void Root_widget<Config, With_layout>::init()
    {
        init_children_resources();

        Abstract_widget_t::init();
    }

    /*
    template<class Config, bool With_layout>
    void Root_widget<Config, With_layout>::set_focus_to(Widget_t *widget)
    {
        if (_focused_widget) _focused_widget->loosing_focus(); // TODO: support veto-ing loss of focus ?
        _focused_widget = widget;
        if (_focused_widget) _focused_widget->gained_focus();
    }
    */

    template<class Config, bool With_layout>
    void Root_widget<Config, With_layout>::push_cursor(Cursor_handle cursor)
    {
        _cursor_stack.push( Config::Mouse::get_current_cursor() );

        Config::Mouse::set_cursor(cursor);
    }

    template<class Config, bool With_layout>
    void Root_widget<Config, With_layout>::pop_cursor()
    {
        // All cursors but the top one must be freed after use
        if (_cursor_stack.size() > 1)
        {
            Config::Mouse::free_cursor( Config::Mouse::get_current_cursor() );
        }

        Config::Mouse::set_cursor(_cursor_stack.top());

        _cursor_stack.pop();
    }

    template<class Config, bool With_layout>
    auto Root_widget<Config, With_layout>::get_font_handle(const Rasterized_font *font) -> Font_handle
    {
        return _font_mapper.get_resource(_canvas, font);
    }

    template<class Config, bool WithLayout>
    void Root_widget<Config, WithLayout>::mouse_motion(const Position &pos)
    {
        this->lock();
        container_mouse_motion(pos); // Abstract_container TODO: better name ?
        this->unlock();
    }

    template<class Config, bool With_layout>
    void Root_widget<Config, With_layout>::mouse_button(const Position &pos, int button, Key_state state)
    {
        this->lock();
        container_mouse_button(pos, button, state); // Abstract_container TODO: better name ?
        this->unlock();
    }

    template<class Config, bool With_layout>
    void Root_widget<Config, With_layout>::mouse_click(const Position &pos, int button, int count)
    {
        this->lock();
        container_mouse_click(pos, button, count);
        this->unlock();
    }

    template<class Config, bool With_layout>
    void Root_widget<Config, With_layout>::mouse_wheel(const Position &dir)
    {
        this->lock();
        container_mouse_wheel(dir);
        this->unlock();
    }

    template<class Config, bool With_layout>
    void Root_widget<Config, With_layout>::text_input(const char32_t *text, size_t size)
    {
        if (focused_child())
        {
            this->lock();
            focused_child()->text_input(text, size);
            this->unlock();
        }
    }

    template<class Config, bool With_layout>
    void Root_widget<Config, With_layout>::key_down(const Keycode &key)
    {
        if (focused_child())
        {
            this->lock();
            focused_child()->key_down(key);
            this->unlock();
        }
    }

    template<class Config, bool With_layout>
    void Root_widget<Config, With_layout>::render()
    {
        render(_canvas, {0, 0});
    }

    template<class Config, bool WithLayout>
    inline void Root_widget<Config, WithLayout>::render(Canvas_t *cv, const Position &offs)
    {
        auto pos = offs + position();

        if (_bkgnd_clr[3] > 0)
        {
            cv->clear(cv->rgba_to_native(_bkgnd_clr));
        }
        
        for (auto& child : children())
        {
            if (child->visible()) child->render(cv, pos);
        }
    }

    // Updater aspect -----------------------------------------------

    template<class Config, bool With_layout>
    template<class Aspect_parent>
    inline void Default__Root_widget__Updater<Config, With_layout>::Aspect<Aspect_parent>::invalidate()
    {
        _on_invalidated();
    }

    // Layouter aspect ----------------------------------------------

    template<class Config>
    template<class Aspect_parent>
    inline void Root_widget__Layouter<Config, true>::Aspect<Aspect_parent>::init_layout()
    {
        p()->init_children_layout();
    }

    template<class Config>
    template<class Aspect_parent>
    inline void Root_widget__Layouter<Config, true>::Aspect<Aspect_parent>::layout()
    {
        p()->layout_children(); 
    }

    template<class Config>
    template<class Aspect_parent>
    inline void Root_widget__Layouter<Config, true>::Aspect<Aspect_parent>::insert_child(Widget_t *child)
    {
        p()->add_child(child);

        child->init_layout();
        child->layout();
        child->init();

        p()->invalidate();
    }

    template<class Config>
    template<class Aspect_parent>
    inline void Root_widget__Layouter<Config, true>::Aspect<Aspect_parent>::drop_child(Widget_t *child)
    {
        p()->remove_child(child); 

        p()->invalidate();
    }

} // ns cppgui