/*  libCppGUI - A GUI library for C++11/14
    
    Copyright 2016 Hans-Peter Gygax

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

#include "./Vertical_slider.hpp"

namespace cppgui {

    // Main class -------------------------------------------------------------

    template<class Config, typename ValueType>
    template<class Class, bool With_layout>
    void _vertical_slider<Config, ValueType>::Base<Class, With_layout>::define_range(const Range<Value> &range)
    {
        define_range(range, range.length() / 10, range.length() / 100);
    }

    template <class Config, typename ValueType>
    template <class Class, bool With_layout>
    void _vertical_slider<Config, ValueType>::Base<Class, With_layout>::define_range(const Range<Value> &range, 
        const Value &incr_major, const Value & incr_minor)
    {
        _range = range;
        assert(incr_major >= incr_minor);
        _incr_major = incr_major;
        _incr_minor = incr_minor;
    }

    template <class Config, typename ValueType>
    template <class Class, bool With_layout>
    void _vertical_slider<Config, ValueType>::Base<Class, With_layout>::set_value(const Value &val)
    {
        assert(val >= _range.from && val <= _range.to);
        
        _value = val;
    }

    template <class Config, typename ValueType>
    template <class Class, bool With_layout>
    auto _vertical_slider<Config, ValueType>::Base<Class, With_layout>::value() -> Value
    {
        return _value;
    }

    template <class Config, typename ValueType>
    template <class Class, bool With_layout>
    void _vertical_slider<Config, ValueType>::Base<Class, With_layout>::on_value_changed(Value_changed_handler handler)
    {
        assert(!_on_value_changed);

        _on_value_changed = handler;
    }

    template<class Config, typename ValueType>
    template<class Class, bool With_layout>
    void _vertical_slider<Config, ValueType>::Base<Class, With_layout>::init()
    {
        Widget_t::init();

        /** TODO: the following is potentially problematic, as the event handler might 
                access another widget that is not initialized yet. The question is whether
                or not that is legal (or should be made so).
         */
        // notify_value_change();
    }

    template<class Config, typename ValueType>
    template<class Class, bool With_layout>
    void _vertical_slider<Config, ValueType>::Base<Class, With_layout>::compute_view_from_data()
    {
        update_thumb_pos();
    }

    template <class Config, typename ValueType>
    template <class Class, bool With_layout>
    void _vertical_slider<Config, ValueType>::Base<Class, With_layout>::change_range(const Range<Value> &range)
    {
        define_range(range);
        update_thumb_pos();
    }

    template <class Config, typename ValueType>
    template <class Class, bool With_layout>
    void _vertical_slider<Config, ValueType>::Base<Class, With_layout>::change_range(const Range<Value> &range, 
        const Value & incr_major, const Value & incr_minor)
    {
        define_range(range, incr_major, incr_minor);
        update_thumb_pos();
    }

    template<class Config, typename ValueType>
    template<class Class, bool With_layout>
    void _vertical_slider<Config, ValueType>::Base<Class, With_layout>::render(Canvas_t *canvas, const Point &offset)
    {
        // TODO: PLACEHOLDER
        this->fill_rect(canvas, this->rectangle(), offset, Canvas_t::rgba_to_native({0.8f, 0, 0.7f, 1}));

        // "Slide" (vertical strip)
        // TODO: color from constexpr method / stylesheet
        this->fill_rect(canvas, _slide_rect, offset + this->position(), Canvas_t::rgba_to_native({ 0, 0, 0, 1 }));

        // Thumb
        auto thclr = _thumb_hovered ? Canvas_t::rgba_to_native({ 1, 1, 1, 0.8f }) : Canvas_t::rgba_to_native({ 0.5f, 0.5f, 0.5f, 0.8f });
        this->fill_rect(canvas, _thumb_rect, offset + this->position() + Point{ 0, _thumb_pos }, thclr);
    }

    template <class Config, typename ValueType>
    template <class Class, bool With_layout>
    void _vertical_slider<Config, ValueType>::Base<Class, With_layout>::mouse_button(const Point& pos, int button, Key_state state, Count clicks)
    {
        if (button == 1)
        {
            if (state == pressed && top_of_slide().contains(pos))
            {
                //std::cerr << "top of slide" << std::endl;
                move_up_major();
            }
            else if (state == pressed && bottom_of_slide().contains(pos))
            {
                //std::cerr << "bottom of slide" << std::endl;
                move_down_major();
            }
            else if (state == pressed && (_thumb_rect + Point{ 0, _thumb_pos }).contains(pos) && !_dragging_thumb)
            {
                start_thumb_drag(pos);
            }
            else if (state == released && _dragging_thumb)
            {
                end_thumb_drag();
            }

            if (state == released && (_thumb_rect + Point{ 0, _thumb_pos }).contains(pos) && !_dragging_thumb)
            {
                this->take_focus();
            }
        }

        Parent_t::mouse_button(pos, button, state, clicks);
    }

    template <class Config, typename ValueType>
    template <class Class, bool With_layout>
    void _vertical_slider<Config, ValueType>::Base<Class, With_layout>::mouse_motion(const Point& pos)
    {
        if (this->hovered())
        {
            if (!_thumb_hovered && (_thumb_rect + Point{ 0, _thumb_pos }).contains(pos))
            {
                _thumb_hovered = true;
                this->invalidate();
            }
            else if (_thumb_hovered && !(_thumb_rect + Point{ 0, _thumb_pos }).contains(pos))
            {
                _thumb_hovered = false;
                this->invalidate();
            }
        }

        if (_dragging_thumb)
        {
            drag_thumb(pos);
        }

        Parent_t::mouse_motion(pos);
    }

    template <class Config, typename ValueType>
    template <class Class, bool With_layout>
    void _vertical_slider<Config, ValueType>::Base<Class, With_layout>::mouse_wheel(const Vector &dist)
    {
        change_value( _value - dist.y * _incr_minor );
        update_thumb_pos();
    }

    template <class Config, typename ValueType>
    template <class Class, bool With_layout>
    void _vertical_slider<Config, ValueType>::Base<Class, With_layout>::key_down(const Keycode & key)
    {
        if      (Keyboard::is_page_up  (key)) move_up_major  ();
        else if (Keyboard::is_page_down(key)) move_down_major();
        else if (Keyboard::is_up       (key)) move_up_minor  ();
        else if (Keyboard::is_down     (key)) move_down_minor();
        else
            Parent_t::key_down(key);
    }

    template <class Config, typename ValueType>
    template <class Class, bool With_layout>
    void _vertical_slider<Config, ValueType>::Base<Class, With_layout>::mouse_exit()
    {
        if (_thumb_hovered)
        {
            _thumb_hovered = false;
            this->invalidate();
        }

        Parent_t::mouse_exit();
    }

    template <class Config, typename ValueType>
    template <class Class, bool With_layout>
    void _vertical_slider<Config, ValueType>::Base<Class, With_layout>::move_down_major()
    {
        change_value( _value + _incr_major );
    }

    template <class Config, typename ValueType>
    template <class Class, bool With_layout>
    void _vertical_slider<Config, ValueType>::Base<Class, With_layout>::move_up_major()
    {
        change_value( _value - _incr_major );
    }

    template <class Config, typename ValueType>
    template <class Class, bool With_layout>
    void _vertical_slider<Config, ValueType>::Base<Class, With_layout>::move_down_minor()
    {
        change_value( _value + _incr_minor );
    }

    template <class Config, typename ValueType>
    template <class Class, bool With_layout>
    void _vertical_slider<Config, ValueType>::Base<Class, With_layout>::move_up_minor()
    {
        change_value( _value - _incr_minor );
    }

    template <class Config, typename ValueType>
    template <class Class, bool With_layout>
    void _vertical_slider<Config, ValueType>::Base<Class, With_layout>::start_thumb_drag(const Point &pos)
    {
        assert(!_dragging_thumb);
        _dragging_thumb = true;
        _thumb_drag_start_pos = pos.y;
        _thumb_drag_start_value = _value;
    }

    template <class Config, typename ValueType>
    template <class Class, bool With_layout>
    void _vertical_slider<Config, ValueType>::Base<Class, With_layout>::end_thumb_drag()
    {
        assert(_dragging_thumb);
        _dragging_thumb = false;
    }

    template <class Config, typename ValueType>
    template <class Class, bool With_layout>
    void _vertical_slider<Config, ValueType>::Base<Class, With_layout>::drag_thumb(const Point &pos)
    {
        auto dy = pos.y - _thumb_drag_start_pos;

        change_value( _thumb_drag_start_value + dy * _range.length() / _slide_rect.height() );
    }

    template <class Config, typename ValueType>
    template <class Class, bool With_layout>
    void _vertical_slider<Config, ValueType>::Base<Class, With_layout>::change_value(const Value &value)
    {
        if      (value < _range.from) _value = _range.from;
        else if (value > _range.to  ) _value = _range.to;
        else                          _value = value;

        notify_value_change();

        update_thumb_pos();
    }

    template <class Config, typename ValueType>
    template <class Class, bool With_layout>
    void _vertical_slider<Config, ValueType>::Base<Class, With_layout>::notify_value_change()
    {
        if (_on_value_changed) _on_value_changed(_value);
    }

    template <class Config, typename ValueType>
    template <class Class, bool With_layout>
    void _vertical_slider<Config, ValueType>::Base<Class, With_layout>::update_thumb_pos()
    {
        _thumb_pos = static_cast<Position>( (_value - _range.from) * _slide_rect.height() / _range.length() );
        std::cerr << "value: " << _value << std::endl;

        this->invalidate();
    }

    template <class Config, typename ValueType>
    template <class Class, bool With_layout>
    auto _vertical_slider<Config, ValueType>::Base<Class, With_layout>::top_of_slide() const -> Rectangle
    {
        Position top = _thumb_rect.top() + _thumb_pos;
        if (top > _slide_rect.top())
        {
            Rectangle res = _slide_rect;
            res.set_bottom( top );
            return res;
        }
        else 
            return {};
    }

    template <class Config, typename ValueType>
    template <class Class, bool With_layout>
    auto _vertical_slider<Config, ValueType>::Base<Class, With_layout>::bottom_of_slide() const -> Rectangle
    {
        auto bottom = _thumb_rect.bottom() + _thumb_pos;
        if (bottom < _slide_rect.bottom())
        {
            Rectangle res = _slide_rect;
            res.set_top( bottom );
            return res;
        }
        else
            return {};
    }

    // Layouter aspect --------------------------------------------------------

    template<class Config, typename ValueType>
    template<class Class, class Parent>
    void _vertical_slider<Config, ValueType>::Layouter<Class, true, Parent>::init_layout()
    {
        /** The init_layout() method is called on the complete widget tree 
            before either get_minimal_size() or layout(). It is intended as 
            an occasion to compute measurements (typically of text strings) 
            that can then be used in both get_minimal_size() and layout().
         */
    }

    template<class Config, typename ValueType>
    template<class Class, class Parent>
    auto _vertical_slider<Config, ValueType>::Layouter<Class, true, Parent>::get_minimal_size() -> Extents
    {
        return {
            std::max( p()->thumb_size().w, p()->slide_width() ),
            5 * p()->thumb_size().h // TODO: less arbitrary definition ?
        };
    }

    template<class Config, typename ValueType>
    template<class Class, class Parent>
    void _vertical_slider<Config, ValueType>::Layouter<Class, true, Parent>::layout()
    {
        /** The layout() method is usually called once, recursively, on the 
            whole widget tree. This is where the Layouter aspect must
            position and size all contained elements.
         */

        auto& exts = p()->extents();
        auto thsize = p()->thumb_size();

        p()->_slide_rect = {
            { (exts.w - p()->slide_width()) / 2, thsize.h / 2 },
            { p()->slide_width(), p()->extents().h - thsize.h }
        };

        p()->_thumb_rect = { (exts.w - thsize.w) / 2, 0, thsize.w, thsize.h };
    }

} // ns cppgui
