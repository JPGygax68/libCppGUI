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
#include "./Box.hpp"
#include "./Vertical_scrollbar.hpp"

namespace cppgui {

    // Forward declarations

    template<class Config, bool With_layout, class Pane, class Parent> struct Scrollbox__Layouter;
    template<class Config, bool With_layout> class Scrollable_pane;

    // Class definition

    // TODO: rename to Scrollbox_base ?

    template<class Config, bool With_layout, class Pane> // = Scrollable_pane<Config, With_layout>>
    class Scrollbox: 
        public Scrollbox__Layouter<Config, With_layout, Pane,
            Bordered_box<Config, With_layout, Container<Config, With_layout> > >
    {
    public:
        using Container_t = Container<Config, With_layout>;
        using Parent_t = Container_t;
        using Canvas_t = typename Canvas<typename Config::Renderer>;
        using Scrollable_pane_t = Pane; // Scrollable_pane<Config, With_layout>;
        using Keyboard = typename Config::Keyboard;
        using Keycode = typename Keyboard::Keycode;
        //using List_pane_t = List_pane_base<Config, With_layout>;

        using Navigation_handler = std::function<void(Navigation_unit)>;

        Scrollbox()
        {
            _vert_sbar.set_focussable(false); // TODO: should a scrollbar be focussable in the first place ?

            /* _vert_sbar.on_position_change([this](const Fraction<> &pos) {

            _content->set_position({ 0, - static_cast<Position>((_content->extents().h - extents().h) * pos.num / pos.den) });
            invalidate();
            }); */

            _vert_sbar.on_navigation([this](Navigation_unit unit, /* Position initial_pos, */ const Fraction<int> &delta) {

                static_cast<Pane*>(_content)->scroll(unit, /* initial_pos, */ delta);
            });

            add_child(&_vert_sbar);
        }

        void init() override
        {
            Container_t::init(); // will also init scrollbar
        }

        void compute_view_from_data() override
        {
            // Content pane must have its size set at this point
            _vert_sbar.define_values(_content->extents().h, _content_rect.ext.h);

            Parent_t::compute_view_from_data();
        }

        void on_navigation(Navigation_handler)
        {
            _on_navigation = handler;
        }

        void set_content_pane(Scrollable_pane_t *content)
        {
            assert(!_content);

            _content = content;
            //_content->set_border({1, Color{ 1, 1, 0, 1 }});

            add_child(_content);

            //_content->set_position({ (Position) _border.width, (Position) _border.width });
        }

        auto content_pane() { return _content; }

        auto& vertical_scrollbar() { return _vert_sbar; }

        void mouse_wheel(const Vector &delta) override
        {
            // TODO: better way than just redirecting input events ? e.g. call Impl::move_by_items() ?
            _vert_sbar.mouse_wheel(delta);
        }

        void key_down(const Keycode &key) override
        {
            if      (Keyboard::is_down     (key)) pane()->scroll(Navigation_unit::element, { 1, 1});
            else if (Keyboard::is_up       (key)) pane()->scroll(Navigation_unit::element, {-1, 1});
            else if (Keyboard::is_page_down(key)) pane()->scroll(Navigation_unit::page   , { 1, 1});
            else if (Keyboard::is_page_up  (key)) pane()->scroll(Navigation_unit::page   , {-1, 1});
            else
                Parent_t::key_down(key);
        }

        void render(Canvas_t *canvas, const Point &offset) override
        {
            Rectangle r { rectangle() + offset };

            canvas->set_clipping_rect(r.pos.x, r.pos.y, r.ext.w, r.ext.h);

            Container_t::render(canvas, offset); // renders the scrollbars and the content pane

            canvas->cancel_clipping();

            draw_border(canvas, offset); //, _border.width, _border.color);

            fill_rect(canvas, { _vert_sep_pos, (Position) _border.width, _separator.width, extents().h - 2 * _border.width }, 
                r.pos, canvas->rgba_to_native(_separator.color) );
        }

        // TODO: it may be better to let the pane access the member directly via friend declaration
        auto content_rectangle() const -> const Rectangle & { return _content_rect; } // to be accessed by Scrollable_pane (or derived)

    protected:
        using Vertical_scrollbar_t = Custom_vertical_scrollbar<Config, With_layout>;

        auto pane() { return static_cast<Scrollable_pane_t*>(_content); }

        //Border                  _border    = { 4, {0, 0.2f, 0.6f, 1} };     // encompasses both content area and scrollbar(s)
        Separator               _separator = { 1, {0.2f, 0.2f, 0.2f, 1} };
        Vertical_scrollbar_t    _vert_sbar;
        Rectangle               _content_rect;      // set by layouter
        Position                _vert_sep_pos;      // positions of the separators (vertical = x, horizontal = y)
        Navigation_handler      _on_navigation;
        Scrollable_pane_t      *_content = nullptr;
    };

    // Layouter aspect

    template<class Config, class Pane, class Parent>
    struct Scrollbox__Layouter<Config, true, Pane, Parent>: public Parent
    {
        struct Scrollbox_t: public Scrollbox<Config, true, Pane> { friend struct Scrollbox__Layouter; };
        auto p() { return static_cast<Scrollbox_t*>(static_cast<Scrollbox<Config, true, Pane>*>(this)); }

        auto get_minimal_size() -> Extents override
        {
            // Somewhat arbitrary: width: twice that of scrollbar, height: scrollbar

            auto sb_minsz = p()->_vert_sbar.get_minimal_size();

            return { 2 * sb_minsz.w, sb_minsz.h };
        }

        void layout() override
        {
            /** TODO: this whole algorithm must also be accessible outside of the layouting aspect,
            because it should be triggered whenever the pane notifies a change in size.
            */

            // Preparations

            auto exts = p()->extents();
            auto vertsb_minsz = p()->_vert_sbar.get_minimal_size();
            //auto horzsb_minsz = p()->_horz_sbar.get_minimal_size();

            // Try without scrollbars first, then add scrollbars as needed

            //bool have_horz_sbar = false;    // TODO: future: may be forced on
            bool have_vert_sbar = false;    // ditto

            Extents cont_exts; // output of this algorithm

            for ( ;; )
            {
                // Compute container extents
                cont_exts = { exts.w - 2 * p()->_border.width, exts.h - 2 * p()->_border.width };
                if (have_vert_sbar) cont_exts.w -= p()->_separator.width + vertsb_minsz.w;
                //if (horz_scrollbar) cont_exts.h -= p()->_separator.width + horzsb_minsz.w;

                // Ask content pane to adjust and check its resulting extents
                p()->_content->compute_and_set_extents( cont_exts );
                auto pane_exts = p()->_content->extents();

                if      (pane_exts.h > cont_exts.h && !have_vert_sbar) have_vert_sbar = true;
                //else if (pane_exts.w > cont_exts.w && !have_horz_sbar) have_horz_sbar = true;
                else 
                    break; // done
            }

            // Enable the scrollbars we need
            p()->_vert_sbar.set_visible( have_vert_sbar );
            //p()->_horz_sbar.set_visible( have_horz_sbar );

            // Position and layout the components:

            // Content pane and its containing rectangle
            p()->_content_rect = Rectangle { Point { (Position) p()->_border.width, (Position) p()->_border.width }, cont_exts };
            p()->_content->set_position({ p()->_content_rect.pos.x, p()->_content_rect.pos.y });
            p()->_content->layout(); // usually won't do much (already done by compute_and_set_extents()

                                     // Scrollbars and their separators
            if (have_vert_sbar)
            {
                p()->_vert_sep_pos = exts.w - p()->_border.width - vertsb_minsz.w - p()->_separator.width;
                p()->_vert_sbar.set_position({ exts.right() - (Position_delta) (vertsb_minsz.w + p()->_border.width), 0 });
                p()->_vert_sbar.set_extents ({ vertsb_minsz.w, exts.h });
                p()->_vert_sbar.layout();
            }
            /* if (have_horz_sbar)
            {
            p()->_horz_sep_pos = exts.h - p()->_border.width - sb_minsz.h - p()->_separator.width;
            p()->_horz_sbar.set_position({ exts.bottom() - (Position_delta) (vertsb_minsz.h + p()->_border.width), 0 });
            p()->_horz_sbar.set_extents ({ exts.w, horzsb_minsz.h });
            p()->_horz_sbar.layout();
            } */
        }

    protected:
        //auto content_rect() -> Rectangle;
    };

    // Scrollable pane ==============================================

    // Forward declarations

    template<class Config, bool With_layout, class Parent> struct Scrollable_pane__Layouter;

    // Base class

    template<class Config, bool With_layout>
    class Scrollable_pane_base: public Container<Config, With_layout>
    {
    public:
        //using Navigation_handler = Custom_vertical_scrollbar<Config, With_layout>;

    protected:
        
        auto scrollbox() { return static_cast<Scrollbox<Config, With_layout, Scrollable_pane_base>*>( container() ); }

        /** To implement in CRTP descendant class.
            The implementation is responsible for calling update_scrollbar_position() on the containing scrollbox.
         */
        void scroll(Navigation_unit unit, Fraction<int> delta) { static_assert(false, "Scrollable_pane::navigate()"); }

        /** TODO: inform container scrollbox that the extents of the pane have changed,
            and to update the scrollbar(s) accordingly
         */
        void notify_extents_changed();

    private:

        void do_scroll(Navigation_unit unit, Fraction<int> delta)
        {
            scroll(unit, delta);
            scrollbox()->update_scrollbar_position();
        }
    };

    // Main class template

    template<class Config, bool With_layout>
    class Scrollable_pane: 
        public Scrollable_pane__Layouter<Config, With_layout, Scrollable_pane_base<Config, With_layout> >
    {
    protected:
        static constexpr auto element_background_color()            { return Color{ 0.8f, 0.8f, 0.8f, 1 }; }
        static constexpr auto element_background_color_selected()   { return Color{ 0, 0.7f, 1, 1 }; }
    };

    // Layouter aspect

    template<class Config, class Parent>
    struct Scrollable_pane__Layouter<Config, true, Parent>: public Parent
    {
        struct Scrollable_pane_t: public Scrollable_pane<Config, true> { friend struct Scrollable_pane__Layouter; };
        auto p() { return static_cast<Scrollable_pane_t*>(static_cast<Scrollable_pane<Config, true>*>(this)); }

        /** Because the size of a scrollable pane can by definition exceed that of its container,
            this additional (CRTP) entry point is provided as an occasion for the pane implementation
            to adapt (in whatever way) to the size of the content rectangle of the scrollbox.
            */
        void compute_and_set_extents(const Extents &content_rect) { static_assert(false, "CRPT"); }

        //auto get_minimal_size() -> Extents override;
        //void layout() override;
    };

} // ns cppgui
