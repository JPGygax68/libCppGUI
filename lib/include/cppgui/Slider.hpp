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

#include "./Widget.hpp"

namespace cppgui {

    // Main class declaration ---------------------------------------

    template<class Config, Orientation Orientation, typename ValueType>
    struct _slider
    {
        template<class Class, bool With_layout, class Parent> struct Layouter;

        // Main class declaration -----------------------------------

        /** The class is called "Base" because it is intended to be customized via template parameters.
         */
        template<class Class, bool With_layout>
        class Base: 
            public Layouter<Class, With_layout,     // Layouter aspect, parameterized with ...
                Widget<Config, With_layout> >       // ... the actual parent class: Widget<>
        {
        public:
            using Value_type    = typename ValueType;

            using Widget_t = Widget<Config, With_layout>;
            using Parent_t = Widget_t;
            using Canvas_t = typename Widget_t::Canvas_t;
            using Keyboard = typename Config::Keyboard;
            using Keycode = typename Keyboard::Keycode;

            using Value_changed_handler = std::function<void(const Value_type &)>;

            void define_range(const Range<Value_type> &);
            void define_range(const Range<Value_type> &, const Value_type &step_major, const Value_type &step_minor);
            void set_value(const Value_type &);
            auto value() -> Value_type;

            void on_value_changed(Value_changed_handler);

            void init() override;

            void compute_view_from_data() override;

            void change_range(const Range<Value_type> &);
            void change_range(const Range<Value_type> &, const Value_type &step_major, const Value_type &step_minor);
            void change_value(const Value_type &);

            void render(Canvas_t *, const Point &offset) override;

            void mouse_button(const Point &, int button, Key_state, Count clicks) override;
            void mouse_motion(const Point &) override;
            void mouse_wheel(const Vector &) override;

            void key_down(const Keycode &key) override;

            void mouse_exit () override;

        protected:

            using Longitudinal_position_t = Directed_position< Axis_reversed<Orientation, Widget_t::y_axis_up>::value >;
            using Oriented_rectangle_t = Oriented_rectangle<Orientation, Default_latitudinal_orientation<Orientation>::value, Widget_t::y_axis_up>;

            static constexpr auto slide_width() -> Width   { return 10; }
            static constexpr auto knob_size  () -> Extents { return { 30, 16 }; }

            auto knob_rectangle() const -> Rectangle;

            // Actions

            void decrease_major();
            void increase_major  ();
            void decrease_minor();
            void increase_minor  ();

            // Internal methods

            void start_knob_drag(const Point &);
            void end_knob_drag  ();
            void drag_knob      (const Point &);

            void notify_value_change();
            void update_knob_pos();

            auto track_portion_after_knob () const -> Rectangle;
            auto track_portion_before_knob() const -> Rectangle;

            Value_changed_handler           _on_value_changed;

            Range<Value_type>               _range;
            Value_type                      _incr_major, _incr_minor;

            Oriented_rectangle_t            _slide_rect;
            Oriented_rectangle_t            _knob_rect;

            Value_type                      _value;
            Position_delta                  _knob_pos;

            bool                            _knob_hovered = false;
            bool                            _dragging_knob = false;
            Position_delta                  _knob_drag_start_pos;
            Value_type                      _knob_drag_start_value;
        };

        // Layouter aspect ----------------------------------------------

        /** Dummy template specialization for when With_layout = false.
         */
        template<class Class, class Parent> struct Layouter<Class, false, Parent>: public Parent {};

        /** "Real" layouter specialization that will be selected when With_layout = true.
         */
        template<class Class, class Parent>
        struct Layouter<Class, true, Parent>: public Parent
        {
            void init_layout() override;

            auto get_minimal_size() -> Extents override;

            void layout() override;

        protected:
            class Class_t: public Class { friend struct Layouter; };

            /** Gives access to the main class, including protected and private sections.
            */
            auto p() { return static_cast<Class_t*>(this); }
        };

    }; // templated ns _vertical_slider

    // Export section -----------------------------------------------

    template<class Config, bool With_layout, typename ValueType = float>
    class Horizontal_slider:
        public _slider<Config, left_to_right, ValueType>::template Base<Horizontal_slider<Config, With_layout, ValueType>, With_layout>
    {
    };

    template<class Config, bool With_layout, typename ValueType = float>
    class Vertical_slider:
        public _slider<Config, bottom_up, ValueType>::template Base<Vertical_slider<Config, With_layout, ValueType>, With_layout>
    {
    };

} // ns cppgui
