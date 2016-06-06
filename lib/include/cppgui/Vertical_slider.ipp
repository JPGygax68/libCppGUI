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
        _range = range;
    }

    template <class Config, typename ValueType>
    template <class Class, bool With_layout>
    void _vertical_slider<Config, ValueType>::Base<Class, With_layout>::set_value(const Value &val)
    {
        assert(val >= _range.from && val <= _range.to);
        _value = val;
    }

    template<class Config, typename ValueType>
    template<class Class, bool With_layout>
    void _vertical_slider<Config, ValueType>::Base<Class, With_layout>::init()
    {
        /** This is where the widget establishes its connection with the backends
            (available via root_widget()->canvas() etc. ).
         */

        update_thumb_pos();

        Widget_t::init();
    }

    template<class Config, typename ValueType>
    template<class Class, bool With_layout>
    void _vertical_slider<Config, ValueType>::Base<Class, With_layout>::compute_view_from_data()
    {
        /** This is another entry point that gets called recursively upon
            initialization. Its name says what it is intended for: to 
            set internal state so as to reflect the bound data.
         */
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
        if (button == 1 && state == pressed && (_thumb_rect + Point{ 0, _thumb_pos }).contains(pos) && !_dragging_thumb)
        {
            start_thumb_drag(pos);
        }
        else if (button == 1 && state == released && _dragging_thumb)
        {
            end_thumb_drag();
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

        Value new_val = _thumb_drag_start_value + dy * _range.length() / _slide_rect.height();

        if      (new_val < _range.from) new_val = _range.from;
        else if (new_val > _range.to  ) new_val = _range.to;

        _value = new_val;

        update_thumb_pos();
    }

    template <class Config, typename ValueType>
    template <class Class, bool With_layout>
    void _vertical_slider<Config, ValueType>::Base<Class, With_layout>::update_thumb_pos()
    {
        _thumb_pos = (_value - _range.from) * _slide_rect.height() / _range.length();
        std::cerr << "value: " << _value << std::endl;

        this->invalidate();
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
        /** The get_minimal_size() method is intended to be called recursively
            by containers, or from a container's layout() method to help it
            decide how to place and size its children.
         */

        return {};
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
