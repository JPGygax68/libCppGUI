#pragma once

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

#include "./Slider.hpp"

#include "./Widget.ipp"

#pragma warning(disable: 4505)

namespace cppgui {

    // Main class -------------------------------------------------------------

    template<class Config, typename ValueType>
    template<class Class, bool With_layout, class GeometryAccessor>
    void _slider<Config, ValueType>::Base<Class, With_layout, GeometryAccessor>::define_range(const Range<Value_type> &range)
    {
        define_range(range, range.length() / 10, range.length() / 100);
    }

    template <class Config, typename ValueType>
    template <class Class, bool With_layout, class GeometryAccessor>
    void _slider<Config, ValueType>::Base<Class, With_layout, GeometryAccessor>::define_range(const Range<Value_type> &range, 
        const Value_type &incr_major, const Value_type & incr_minor)
    {
        _range = range;
        assert(incr_major >= incr_minor);
        _incr_major = incr_major;
        _incr_minor = incr_minor;
    }

    template <class Config, typename ValueType>
    template <class Class, bool With_layout, class GeometryAccessor>
    void _slider<Config, ValueType>::Base<Class, With_layout, GeometryAccessor>::set_value(const Value_type &val)
    {
        assert(val >= _range.from && val <= _range.to);
        
        _value = val;
    }

    template <class Config, typename ValueType>
    template <class Class, bool With_layout, class GeometryAccessor>
    auto _slider<Config, ValueType>::Base<Class, With_layout, GeometryAccessor>::value() -> Value_type
    {
        return _value;
    }

    template <class Config, typename ValueType>
    template <class Class, bool With_layout, class GeometryAccessor>
    void _slider<Config, ValueType>::Base<Class, With_layout, GeometryAccessor>::on_value_changed(Value_changed_handler handler)
    {
        assert(!_on_value_changed);

        _on_value_changed = handler;
    }

    template<class Config, typename ValueType>
    template<class Class, bool With_layout, class GeometryAccessor>
    void _slider<Config, ValueType>::Base<Class, With_layout, GeometryAccessor>::init()
    {
        Widget_t::init();

        /** TODO: the following is potentially problematic, as the event handler might 
                access another widget that is not initialized yet. The question is whether
                or not that is legal (or should be made so).
         */
        // notify_value_change();
    }

    template<class Config, typename ValueType>
    template<class Class, bool With_layout, class GeometryAccessor>
    void _slider<Config, ValueType>::Base<Class, With_layout, GeometryAccessor>::compute_view_from_data()
    {
        update_knob_pos();
    }

    template <class Config, typename ValueType>
    template <class Class, bool With_layout, class GeometryAccessor>
    void _slider<Config, ValueType>::Base<Class, With_layout, GeometryAccessor>::change_range(const Range<Value_type> &range)
    {
        define_range(range);
        update_knob_pos();
    }

    template <class Config, typename ValueType>
    template <class Class, bool With_layout, class GeometryAccessor>
    void _slider<Config, ValueType>::Base<Class, With_layout, GeometryAccessor>::change_range(const Range<Value_type> &range, 
        const Value_type & incr_major, const Value_type & incr_minor)
    {
        define_range(range, incr_major, incr_minor);
        update_knob_pos();
    }

    template<class Config, typename ValueType>
    template<class Class, bool With_layout, class GeometryAccessor>
    void _slider<Config, ValueType>::Base<Class, With_layout, GeometryAccessor>::render(Canvas_t *canvas, const Point &offset)
    {
        // TODO: PLACEHOLDER
        //this->fill_rect(canvas, this->rectangle(), offset, Canvas_t::rgba_to_native({0.8f, 0, 0.7f, 1}));

        auto pos = offset + this->position();

        if (this->has_focus())
        {
            auto ext = this->extents();
            canvas->draw_stippled_rectangle_outline(pos.x, pos.y, ext.w, ext.h, {0, 0, 0.5f, 1});
        }

        // "Slide" (vertical strip)
        // TODO: color from constexpr method / stylesheet
        this->fill_rect(canvas, _slide_rect, pos, Canvas_t::rgba_to_native({ 0.3f, 0.3f, 0.3f, 1 }));

        // Thumb
        auto thclr = _knob_hovered ? Canvas_t::rgba_to_native({ 1, 1, 1, 1 }) : Canvas_t::rgba_to_native({ 0.7f, 0.7f, 0.7f, 1 });
        this->fill_rect(canvas, _knob_rect, pos + _slide_rect.vector_from_first_axis_start(_knob_pos), thclr);
    }

    template <class Config, typename ValueType>
    template <class Class, bool With_layout, class GeometryAccessor>
    void _slider<Config, ValueType>::Base<Class, With_layout, GeometryAccessor>::mouse_button(const Point& pos, int button, Key_state state, Count clicks)
    {
        if (button == 1)
        {
            if (state == pressed && track_portion_after_knob().contains(pos))
            {
                //std::cerr << "top of slide" << std::endl;
                increase_major();
            }
            else if (state == pressed && track_portion_before_knob().contains(pos))
            {
                //std::cerr << "bottom of slide" << std::endl;
                decrease_major();
            }
            else if (state == pressed && knob_rectangle().contains(pos) && !_dragging_knob)
            {
                start_knob_drag(pos);
            }
            else if (state == released && _dragging_knob)
            {
                end_knob_drag();
            }

            if (state == released && knob_rectangle().contains(pos) && !_dragging_knob)
            {
                this->take_focus();
            }
        }

        Parent_t::mouse_button(pos, button, state, clicks);
    }

    template <class Config, typename ValueType>
    template <class Class, bool With_layout, class GeometryAccessor>
    void _slider<Config, ValueType>::Base<Class, With_layout, GeometryAccessor>::mouse_motion(const Point& pos)
    {
        if (this->hovered())
        {
            if (!_knob_hovered && knob_rectangle().contains(pos))
            {
                _knob_hovered = true;
                this->invalidate();
            }
            else if (_knob_hovered && !knob_rectangle().contains(pos))
            {
                _knob_hovered = false;
                this->invalidate();
            }
        }

        if (_dragging_knob)
        {
            drag_knob(pos);
        }

        Parent_t::mouse_motion(pos);
    }

    template <class Config, typename ValueType>
    template <class Class, bool With_layout, class GeometryAccessor>
    void _slider<Config, ValueType>::Base<Class, With_layout, GeometryAccessor>::mouse_wheel(const Vector &dist)
    {
        //change_value( _value + this->forward_position(dist) * _incr_minor );
        change_value( _value + dist.y * _incr_minor ); // TODO: support horizontal wheel ? (? does that exist ?)
        update_knob_pos();
    }

    template <class Config, typename ValueType>
    template <class Class, bool With_layout, class GeometryAccessor>
    void _slider<Config, ValueType>::Base<Class, With_layout, GeometryAccessor>::key_down(const Keycode & key)
    {
        if      (Keyboard::is_page_up  (key)) increase_major();
        else if (Keyboard::is_page_down(key)) decrease_major();
        else if (Keyboard::is_up       (key)) increase_minor();
        else if (Keyboard::is_down     (key)) decrease_minor();
        else
            Parent_t::key_down(key);
    }

    template <class Config, typename ValueType>
    template <class Class, bool With_layout, class GeometryAccessor>
    void _slider<Config, ValueType>::Base<Class, With_layout, GeometryAccessor>::mouse_exit()
    {
        if (_knob_hovered)
        {
            _knob_hovered = false;
            this->invalidate();
        }

        Parent_t::mouse_exit();
    }

    template <class Config, typename ValueType>
    template <class Class, bool With_layout, class GeometryAccessor>
    auto _slider<Config, ValueType>::Base<Class, With_layout, GeometryAccessor>::knob_rectangle() const -> Rectangle
    {
        return _knob_rect + _slide_rect.vector_from_first_axis_start(_knob_pos);
    }

    template <class Config, typename ValueType>
    template <class Class, bool With_layout, class GeometryAccessor>
    void _slider<Config, ValueType>::Base<Class, With_layout, GeometryAccessor>::decrease_major()
    {
        //change_value( _value + _incr_major );
        change_value( _value - _incr_major );
    }

    template <class Config, typename ValueType>
    template <class Class, bool With_layout, class GeometryAccessor>
    void _slider<Config, ValueType>::Base<Class, With_layout, GeometryAccessor>::increase_major()
    {
        //change_value( _value - _incr_major );
        change_value( _value + _incr_major );
    }

    template <class Config, typename ValueType>
    template <class Class, bool With_layout, class GeometryAccessor>
    void _slider<Config, ValueType>::Base<Class, With_layout, GeometryAccessor>::decrease_minor()
    {
        //change_value( _value + _incr_minor );
        change_value( _value - _incr_minor );
    }

    template <class Config, typename ValueType>
    template <class Class, bool With_layout, class GeometryAccessor>
    void _slider<Config, ValueType>::Base<Class, With_layout, GeometryAccessor>::increase_minor()
    {
        //change_value( _value - _incr_minor );
        change_value( _value + _incr_minor );
    }

    template <class Config, typename ValueType>
    template <class Class, bool With_layout, class GeometryAccessor>
    void _slider<Config, ValueType>::Base<Class, With_layout, GeometryAccessor>::start_knob_drag(const Point &pos)
    {
        assert(!_dragging_knob);
        _dragging_knob = true;
        _knob_drag_start_pos = Oriented_point<bottom_up>{pos}.first_axis_position(); // this->forward_position(pos);
        _knob_drag_start_value = _value;
    }

    template <class Config, typename ValueType>
    template <class Class, bool With_layout, class GeometryAccessor>
    void _slider<Config, ValueType>::Base<Class, With_layout, GeometryAccessor>::end_knob_drag()
    {
        assert(_dragging_knob);
        _dragging_knob = false;
    }

    template <class Config, typename ValueType>
    template <class Class, bool With_layout, class GeometryAccessor>
    void _slider<Config, ValueType>::Base<Class, With_layout, GeometryAccessor>::drag_knob(const Point &pos)
    {
        auto delta = Oriented_point<bottom_up>{pos}.first_axis_position() - _knob_drag_start_pos;

        //change_value( _knob_drag_start_value + delta * _range.length() / _slide_rect.height() );
        change_value( _knob_drag_start_value - delta * _range.length() / _slide_rect.length() );
    }

    template <class Config, typename ValueType>
    template <class Class, bool With_layout, class GeometryAccessor>
    void _slider<Config, ValueType>::Base<Class, With_layout, GeometryAccessor>::change_value(const Value_type &value)
    {
        if      (value < _range.from) _value = _range.from;
        else if (value > _range.to  ) _value = _range.to;
        else                          _value = value;

        notify_value_change();

        update_knob_pos();
    }

    template <class Config, typename ValueType>
    template <class Class, bool With_layout, class GeometryAccessor>
    void _slider<Config, ValueType>::Base<Class, With_layout, GeometryAccessor>::notify_value_change()
    {
        if (_on_value_changed) _on_value_changed(_value);
    }

    template <class Config, typename ValueType>
    template <class Class, bool With_layout, class GeometryAccessor>
    void _slider<Config, ValueType>::Base<Class, With_layout, GeometryAccessor>::update_knob_pos()
    {
        _knob_pos = static_cast<Position>( (_value - _range.from) * _slide_rect.length() / _range.length() );

        this->invalidate();
    }

    template <class Config, typename ValueType>
    template <class Class, bool With_layout, class GeometryAccessor>
    auto _slider<Config, ValueType>::Base<Class, With_layout, GeometryAccessor>::track_portion_after_knob() const -> Rectangle
    {
        if (_knob_pos > 0)
        {
            auto res = _slide_rect;
            res.move_first_axis_start_by( _knob_pos + knob_size().h / 2 );
            return res;
        }
        else 
            return {};
    }

    template <class Config, typename ValueType>
    template <class Class, bool With_layout, class GeometryAccessor>
    auto _slider<Config, ValueType>::Base<Class, With_layout, GeometryAccessor>::track_portion_before_knob() const -> Rectangle
    {
        if (_knob_pos < _slide_rect.length())
        {
            auto res = _slide_rect;
            res.set_first_axis_length_to( _knob_pos - knob_size().h / 2 );
            return res;
        }
        else
            return {};
    }

    // Layouter aspect --------------------------------------------------------

    template<class Config, typename ValueType>
    template<class Class, class Parent>
    void _slider<Config, ValueType>::Layouter<Class, true, Parent>::init_layout()
    {
        /** The init_layout() method is called on the complete widget tree 
            before either get_minimal_size() or layout(). It is intended as 
            an occasion to compute measurements (typically of text strings) 
            that can then be used in both get_minimal_size() and layout().
         */
    }

    template<class Config, typename ValueType>
    template<class Class, class Parent>
    auto _slider<Config, ValueType>::Layouter<Class, true, Parent>::get_minimal_size() -> Extents
    {
        Oriented_extents<bottom_up> size; // TODO: adapt to template parameter

        size.length () = 5 * p()->knob_size().h; // TODO: less arbitrary definition ?
        size.width() = p()->knob_size().w;      

        return size;
    }

    template<class Config, typename ValueType>
    template<class Class, class Parent>
    void _slider<Config, ValueType>::Layouter<Class, true, Parent>::layout()
    {
        /** The layout() method is usually called once, recursively, on the 
            whole widget tree. This is where the Layouter aspect must
            position and size all contained elements.
         */

        Oriented_extents<bottom_up> ext{ p()->extents() };
        Oriented_rectangle<bottom_up> rect{ p()->extents() };
        Oriented_extents<bottom_up> knob_size{ p()->knob_size() };

        p()->_slide_rect = rect.define_relative_rectangle( knob_size.h / 2, rect.length() - knob_size.h,
            (rect.width() - p()->slide_width()) / 2, p()->slide_width() );

        //Oriented_rectangle<bottom_up> knob_rect;
        //knob_rect.define_first_edge ( 0,  knob_size.h );
        //knob_rect.define_second_edge( (ext.second_axis_length() - knob_size.w) / 2, knob_size.w );
        //p()->_knob_rect = knob_rect;
        p()->_knob_rect = Oriented_rectangle<bottom_up>{ 
            Oriented_point  <bottom_up>{ 0, (ext.width() - knob_size.w) / 2 }, 
            Oriented_extents<bottom_up>{ knob_size.h, knob_size.w } 
        };
    }

} // ns cppgui
