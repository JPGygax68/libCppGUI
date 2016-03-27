#include <cppgui/Root_widget.hpp>

namespace cppgui {

    template<class Config, bool With_layout>
    Root_widget<Config, With_layout>::Root_widget(Canvas_t *r)
    {
        _canvas = r;
    }

    /*
    template<class Config, bool WithLayout>
    void Root_widget<Config, WithLayout>::add_child(Widget<Config, WithLayout> *child)
    {
        _add_child(child);
        child->added_to_container(static_cast<Abstract_container_t*>(this));
    }
    */

    template<class Config, bool With_layout>
    void Root_widget<Config, With_layout>::init()
    {
        init_children_resources();

        Abstract_widget_t::init();
    }

    template<class Config, bool With_layout>
    void Root_widget<Config, With_layout>::set_focus_to(Widget_t *widget)
    {
        if (_focused_widget) _focused_widget->loosing_focus(); // TODO: support veto-ing loss of focus ?
        _focused_widget = widget;
        if (_focused_widget) _focused_widget->gained_focus();
    }

    template<class Config, bool With_layout>
    void Root_widget<Config, With_layout>::push_cursor(Cursor_handle cursor)
    {
        _cursor_stack.push( Config::Cursor::get_current_cursor() );

        Config::Cursor::set_cursor(cursor);
    }

    template<class Config, bool With_layout>
    void Root_widget<Config, With_layout>::pop_cursor()
    {
        // All cursors but the top one must be freed after use
        if (_cursor_stack.size() > 1)
        {
            Config::Cursor::free_cursor( Config::Cursor::get_current_cursor() );
        }

        Config::Cursor::set_cursor(_cursor_stack.top());

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
        container_mouse_wheel(dir);
    }

    template<class Config, bool With_layout>
    void Root_widget<Config, With_layout>::text_input(const char32_t *text, size_t size)
    {
        if (_focused_widget)
        {
            this->lock();
            _focused_widget->text_input(text, size);
            this->unlock();
        }
    }

    template<class Config, bool With_layout>
    void Root_widget<Config, With_layout>::key_down(const Keycode &key)
    {
        if (_focused_widget)
        {
            this->lock();
            _focused_widget->key_down(key);
            this->unlock();
        }
    }

    template<class Config, bool WithLayout>
    inline void Root_widget<Config, WithLayout>::render(Canvas_t *r, const Position &offs)
    {
        auto pos = offs + position();

        // TODO: the following is temporary - it must be made optional
        r->clear(r->rgba_to_native({ 0, 0.5f, 0.2f, 1 }));
        
        for (auto& child : children())
        {
            child->render(r, pos);
        }
    }

    // Layouter aspect ----------------------------------------------

    template <class Config>
    template<class Aspect_parent>
    inline void Root_widget_layouter<Config, true>::Aspect<Aspect_parent>::init_layout()
    {
        auto p = static_cast<Root_widget_t*>(this);

        p->init_children_layout();
    }

    template <class Config>
    template<class Aspect_parent>
    inline void Root_widget_layouter<Config, true>::Aspect<Aspect_parent>::layout()
    {
        auto p = static_cast<Root_widget_t*>(this);

        p->layout_children(); 
    }

} // ns cppgui