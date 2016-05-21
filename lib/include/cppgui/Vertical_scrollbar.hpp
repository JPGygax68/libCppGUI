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

#include "./Container.hpp"
#include "./Glyph_button.hpp"
#include "./Resource.hpp"
//#include "./Drag_controller.hpp"

namespace cppgui {

    // Forward declarations

    template <class Impl, class Config, bool With_layout, class Parent> struct Vertical_scrollbar__Layouter;

    // Base class ===================================================

    // Resources struct

    /*
    namespace {

        namespace {
            // TODO: make this adaptive in case color does need translating
            struct slide_bgcol        : Non_mapped_resource<Color> {};
            struct thumb_color        : Non_mapped_resource<Color> {};
            struct thumb_hovered_color: Non_mapped_resource<Color> {};
        }

        template<class Config>
        using Resources = Resource_struct<slide_bgcol, thumb_color, thumb_hovered_color>;
    }
    */

    // Class declaration

    /** TODO: the current implementation works by maintaining a bidirectional, "locked"
            relationship between the position of the thumb and the value it represents.
            This is not necessarily the best approach, since the scrollbar's output value 
            can have a significantly higher resolution than the sliding distance of the
            scrollbar's thumb.
            A better implementation could store the output value as a separate (rational)
            value, and do a one-way synchronization from that value to the thumb's
            position, under the control of the consumer.
     */
    template<class Impl, class Config, bool With_layout>
    class Vertical_scrollbar_base: 
        public Vertical_scrollbar__Layouter<Impl, Config, With_layout, Container<Config, With_layout> >
    {
    public:
        using Widget_t = typename Widget<Config, With_layout>;
        using Container_t = typename Container<Config, With_layout>;
        using Canvas_t = typename Widget_t::Canvas_t;

        using Native_color = typename Widget_t::Native_color;

        using Navigation_handler = std::function<void(Navigation_unit, const Fraction<int> &delta)>;

        Vertical_scrollbar_base():
            // TODO: obtain from static value or stylesheet:
            _slide_bgcol        ({ 0.7f, 0.7f, 0.7f, 1 }), 
            _thumb_color        ({ 0.8f, 0.8f, 0.8f, 1 }),
            _thumb_hovered_color({ 0.9f, 0.9f, 0.9f, 1 })
        {
            //_up_btn  .enable_border(false);
            //_down_btn.enable_border(false);
            _up_btn  .set_border({0});
            _down_btn.set_border({0});

            _up_btn  .set_glyph(cppgui::Icon_resources<24>::up_arrow  ());
            _down_btn.set_glyph(cppgui::Icon_resources<24>::down_arrow());

            _up_btn  .set_focussable(false);
            _down_btn.set_focussable(false);

            _up_btn  .on_pushed([this]() { static_cast<Impl*>(this)->move_by_elements(-1); });
            _down_btn.on_pushed([this]() { static_cast<Impl*>(this)->move_by_elements( 1); });

            add_child(&_up_btn  );
            add_child(&_down_btn);
        }

        /** Defines the values that the scrollbar is going to represent, consisting of 
            the "full" range and the "fraction" range. The former will be represented
            by the "slide" between the the up and down buttons, while the latter is
            reflected by the length of the "thumb".

            Note that this does *not* work as a slider: the highest value that can
            be defined and/or represented by the scrollbar is (full - fraction),
            NOT full.
         */
        void define_values(Length full, Length fraction)
        {
            // assert(full >= fraction);
            if (full < fraction) full = fraction; // TODO: better way ?

            _full_range = full;
            _fraction = fraction;
        }

        void init() override
        {
            Container_t::init();

            _slide_bgcol        .translate( root_widget()->canvas() );
            _thumb_color        .translate( root_widget()->canvas() );
            _thumb_hovered_color.translate( root_widget()->canvas() );
        }

        void compute_view_from_data() override
        {
            _thumb_rect.pos = { 2, _sliding_range.p };
            // TODO: position from style

            recalc_thumb();
        }

        void mouse_button(const Point &pos, int button, Key_state state) override
        {
            if (button == 1 && state == Key_state::pressed)
            {
                if (pos.y > _sliding_range.start() && pos.y < _sliding_range.end())
                {
                    auto y_rel = pos.y - _thumb_rect.pos.y;

                    // Above thumb ?
                    if (y_rel < 0)
                    {
                        static_cast<Impl*>(this)->move_by_page(-1);
                    }
                    // Below thumb ?
                    else if (y_rel > _thumb_rect.ext.bottom())
                    {
                        static_cast<Impl*>(this)->move_by_page(1);
                    }
                    else // On thumb
                    {
                        /*_cur_drag_pos = */ _drag_anchor_pos = pos.y;
                        _dragging_thumb = true;
                        root_widget()->capture_mouse(this);
                    }

                    return; // done with this event
                }
            }
            else if (button == 1 && state == Key_state::released)
            {
                if (_dragging_thumb)
                {
                    _dragging_thumb = false;
                    root_widget()->release_mouse();
                }
            }

            Container_t::mouse_button(pos, button, state);
        }
        void mouse_motion(const Point &pos) override
        {
            std::cout << "mouse_motion: " << pos.x << ", " << pos.y << std::endl;

            bool on_thumb = _thumb_rect.contains(pos);
            if (on_thumb != _thumb_hovered)
            {
                _thumb_hovered = on_thumb;
                invalidate();
            }

            if (Config::Mouse::is_button_down(1) && _dragging_thumb)
            {
                std::cerr << "delta = " << (pos.y - _drag_anchor_pos) << std::endl;
                notify_drag_navigation(pos.y - _drag_anchor_pos); // TODO: rename method ?
            }
            else
                Container_t::mouse_motion(pos);
        }
        void mouse_wheel(const Vector &delta) override
        {
            static_cast<Impl*>(this)->move_by_elements(-delta.y);
        }
        void mouse_exit() override
        {
            if (_thumb_hovered)
            {
                // TODO: reaction of some kind ?
                _thumb_hovered = false;
            }

            Container_t::mouse_exit();
        }

        void render(Canvas_t *canvas, const Point &offset) override
        {
            // Background
            // TODO: only draw the part not covered by the buttons ?
            fill(canvas, offset, _slide_bgcol.get() ); 

            // Thumb
            fill_rect(canvas, _thumb_rect, offset + position(), _thumb_hovered ? _thumb_hovered_color.get() : _thumb_color.get() );

            // Children: up and down buttons
            Container_t::render(canvas, offset);
        }

        void change_values(Length range, Length thumb)
        {
            define_values(full, shown);
            recalc_thumb();
            invalidate();
        }
        auto full_range() const { return _full_range; }
        auto fraction() const { return _fraction; }

        auto current_position() -> Fraction<>
        {
            if (_sliding_range.l > _thumb_rect.ext.h)
            {
                return {
                    (_full_range - _fraction) * static_cast<Length>(_thumb_rect.pos.y - _sliding_range.start()),
                    _sliding_range.l - _thumb_rect.ext.h
                };
            }
            else
                return { 0, 1 };
        }
        void update_position(Position pos)
        {
            assert(_full_range >= _fraction);

            if (_full_range > _fraction)
            {
                if (pos < 0) pos = 0; else if (pos > _full_range - _fraction) pos = _full_range - _fraction;

                auto new_pos = _sliding_range.p + (Position) (pos * (_sliding_range.l - _thumb_rect.ext.h) / (_full_range - _fraction));

                if (_dragging_thumb)
                {
                    _drag_anchor_pos += new_pos - _thumb_rect.pos.y;
                }

                _thumb_rect.pos.y = new_pos;
            }

            invalidate();
        }

    protected:
        using Glyph_button_t = typename Glyph_button<Config, With_layout>;
        using Color_resource = typename Widget_t::Color_resource;

        void move_thumb_to(Position)
        {
            new_pos = std::max(new_pos, _sliding_range.start());
            new_pos = std::min(new_pos, _sliding_range.end() - static_cast<Position>(_thumb_rect.ext.h));

            _thumb_rect.pos = {2, new_pos};

            //notify_position_change();
            invalidate();
        }
        void recalc_thumb()
        {
            _thumb_rect.ext.h = std::max(_sliding_range.l * _fraction / _full_range, _thumb_rect.ext.w / 2);

            clip_thumb_pos();
        }
        void clip_thumb_pos()
        {
            if (_thumb_rect.bottom() > _sliding_range.end())
            {
                _thumb_rect.pos.y = _sliding_range.end() - static_cast<Position>(_thumb_rect.ext.h);
            }
            else if (_thumb_rect.pos.y < _sliding_range.start())
            {
                _thumb_rect.pos.y = _sliding_range.start();
            }
        }
        void notify_drag_navigation(const Position_delta &delta)
        {
            if (delta != 0)
            {
                //std::cerr << "delta = " << delta << std::endl;
                //static_cast<Impl*>(this)->move_by_fraction(_drag_start_pos, { delta, static_cast<int>(_sliding_range.l - _thumb_rect.ext.h) });
                static_cast<Impl*>(this)->move_by_fraction({ delta, static_cast<int>(_sliding_range.l - _thumb_rect.ext.h) });
            }
        }

        Color_resource          _slide_bgcol;
        Color_resource          _thumb_color;
        Color_resource          _thumb_hovered_color;

        Glyph_button_t          _up_btn, _down_btn;
        Position                _full_range = 0;    // the range represented by the full length of the "slide" (= strip on which the thumb moves)
        Position                _fraction = 0;      // the range represented by the length of the thumb

        Range                   _sliding_range;
        Rectangle               _thumb_rect;

        bool                    _thumb_hovered = false;
        Position                _drag_anchor_pos;
        Position                _curr_drag_pos;
        bool                    _dragging_thumb = false;
    };

    // Layouter aspect

    template <class Impl, class Config, class Parent>
    struct Vertical_scrollbar__Layouter<Impl, Config, true, Parent>: public Parent 
    {
        // Layouter contract

        //void init_layout() override;

        auto get_minimal_size() -> Extents override
        {
            // Width: based on up/down buttons, height: 3 times button height

            auto btn_minsz = p()->_up_btn  .get_minimal_size();

            return { btn_minsz.w, 3 * btn_minsz.h };
        }

        void layout() override
        {
            auto ext = p()->extents();

            auto minsz_up_btn   = p()->_up_btn  .get_minimal_size();
            auto minsz_down_btn = p()->_down_btn.get_minimal_size();

            p()->_up_btn  .set_rectangle_nw({0, 0           }, { ext.w, minsz_up_btn  .h} );
            p()->_down_btn.set_rectangle_sw({0, ext.bottom()}, { ext.w, minsz_down_btn.h} );

            p()->_sliding_range.define( p()->_up_btn.rectangle().bottom() + 2, p()->_down_btn.rectangle().top() - 2 );

            p()->_thumb_rect.pos.x = 2; // TODO: obtain margin from style
            p()->_thumb_rect.ext.w = p()->extents().w - 4; // obtain margin from style
                                                           //p()->_thumb_rect.ext.h = 20; // TODO: REMOVE, temporary

            p()->_up_btn  .layout();
            p()->_down_btn.layout();
        }

        // Extra capabilities coming with layouting
        // TODO

    protected:
        class Vertical_scrollbar_base_t: public Vertical_scrollbar_base<Impl, Config, true> { friend struct Vertical_scrollbar__Layouter; };

        auto p() { return static_cast<Vertical_scrollbar_base_t*>(static_cast<Vertical_scrollbar_base<Impl, Config, true>*>(this)); }
    };

    // Customizable specialization ==================================

    template<class Config, bool With_layout>
    class Custom_vertical_scrollbar:
        public Vertical_scrollbar_base<
            Custom_vertical_scrollbar<Config, With_layout>, Config, With_layout>
    {
    public:
        void on_navigation(Navigation_handler handler)
        {
            _nav_handler = handler;
        }

    protected:
        friend class Vertical_scrollbar_base<Custom_vertical_scrollbar, Config, With_layout>;

        void move_by_page(int delta)
        {
            // Note: the nav handler must update the position in response

            if (_nav_handler)
            {
                _nav_handler(Navigation_unit::page, /* current_position(), */ { delta, 1 });
                //invalidate();
            }
        }
        void move_by_elements(int delta)
        {
            if (_nav_handler)
            {
                _nav_handler(Navigation_unit::element, /* current_position(), */ { delta, 1 });
                //invalidate();
            }
        }
        //void move_by_fraction(Position initial_pos, const Fraction<int> &delta);
        void move_by_fraction(const Fraction<int> &delta)
        {
            if (_nav_handler)
            {
                _nav_handler(Navigation_unit::fraction, delta);
                //invalidate();
            }
        }

        Navigation_handler      _nav_handler;
    };

    // Standalone specialization ===================================

    template<class Config, bool With_layout>
    class Vertical_scrollbar: public Vertical_scrollbar_base<Vertical_scrollbar<Config, With_layout>, Config, With_layout>
    {
    public:
        using Position_change_handler = std::function<void(Position)>;

        void on_position_change(Position_change_handler handler)
        {
            _on_pos_chng = handler;
        }

        void move_by_page(int pages)
        {
            auto delta = static_cast<cppgui::Position_delta>(fraction()) * pages;
            update_position(current_position() + delta);
            notify_position_change();
        }

        void move_by_elements(int elements)
        {
            update_position(current_position() + elements * 10); // TODO: make step configurable
            notify_position_change();
            invalidate();
        }

        //void move_by_fraction(Position initial_pos, const Fraction<int> &delta);
        
        void move_by_fraction(const Fraction<int> &frac)
        {
            auto delta = full_range() * frac.num / frac.den;
            if (delta == 0) delta = frac.num * frac.den < 0 ? -1 : 1;
            auto thumb_pos = current_position();
            auto new_pos = (Position) (thumb_pos.num + delta * (Position) thumb_pos.den) / (Position) thumb_pos.den;
            update_position( new_pos );
            notify_position_change();
        }

    protected:

        void notify_position_change()
        {
            if (_on_pos_chng) _on_pos_chng(current_position());
        }

        Position_change_handler _on_pos_chng;
    };

} // ns cppgui

#define CPPGUI_INSTANTIATE_VERTICAL_SCROLLBAR(Config, With_layout) \
    template cppgui::Vertical_scrollbar       <Config, With_layout>; \
    /* template cppgui::Vertical_scrollbar_base  <cppgui::Vertical_scrollbar<Config, With_layout>, Config, With_layout>; */ \
    template cppgui::Custom_vertical_scrollbar<Config, With_layout>;
