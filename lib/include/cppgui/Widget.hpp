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

#include <cassert>
#include <type_traits>
#include <initializer_list>

#include <gpc/gui/renderer.hpp> // TODO: other source & namespace

#include "./basic_types.hpp"
#include "./geometry.hpp"
#include "./layouting.hpp"

//#include "./Stylesheet.hpp"
#include "./Resource.hpp"
#include "./Full_resource_mapper.hpp"

namespace cppgui {

    struct Nil_struct {}; // to end aspect chains

    // Forward declarations 

    template <class Config, bool With_layout> class Root_widget;
    template <class Config, bool With_layout> class Abstract_container;
    /*template <class Config, bool With_layout>*/ class Drag_controller;

    enum Key_state { pressed, released }; // TODO: move to basic_types.hpp ?

    template <class Renderer> class Canvas;

    /** Abstract_widget: functionality common to both Root_widget and Widget, i.e. not including the ability
        to function as an element in a container.
     */
    template <class Config, bool With_layout>
    class Abstract_widget
    {
    public:
        using Abstract_widget_t = Abstract_widget;
        using Root_widget_t     = Root_widget<Config, With_layout>;
        using Canvas_t          = typename Canvas<typename Config::Renderer>;
        using Native_color      = typename Canvas_t::Native_color;
        using Font_handle       = typename Canvas_t::Font_handle;
        using Keyboard          = typename Config::Keyboard;
        using Keycode           = typename Keyboard::Keycode;
        // TODO: move the following resource type definitions into a special struct and inherit from that ?
        using Color_resource    = Resource<const Color &, Native_color, Canvas_t, true>;
        using Font_resource     = Resource<const Rasterized_font *, Font_handle, Canvas_t, false>;

        using Click_handler     = std::function<void(const Point &, int button, int clicks)>; // TODO: support return value ?
        using Pushed_handler    = std::function<void()>; // for buttons TODO: renamed event to "Push" (as in "a push happened") ?

        auto& rectangle() { return _rect; }
        auto& rectangle() const { return _rect; }
        auto& position() { return _rect.pos; }
        auto& position() const { return _rect.pos; }
        auto& extents() { return _rect.ext; }
        auto& extents() const { return _rect.ext; }
        void set_position(const Point &pos)
        {
            _rect.pos = pos;
        }
        void set_extents(const Extents &ext)
        {
            _rect.ext = ext;
        }

        // TODO: color and other style definitions belong into stylesheets
        //static auto button_face_color        () { return Color{ 0.8f, 0.8f, 0.8f, 1 }; }
        //static auto button_face_hovered_color() { return Color{ 0.9f, 0.9f, 0.9f, 1 }; }

        /** The init() entry point is where a widget "connects" to its backends (the most important of
            which being the canvas).
         */
        virtual void init() {}

        /** The update_view_from_data() entry point must be called after init(), and also after 
            layout() if run-time layouting is enabled.
         */
        virtual void compute_view_from_data() {}

        // Input event injection

        /** By convention, mouse positions are passed to a widget as relative to
            their own origin (meaning that it falls to the caller, i.e. usually
            the container, to subtract the child widget's position() from the
            coordinates it gets from yet higher up).
         */
        virtual void mouse_motion(const Point &) {}
        virtual void mouse_button(const Point &, int /*button*/, Key_state) {}
        virtual void mouse_click(const Point &, int /*button*/, int /*count*/) {}
        virtual void mouse_wheel(const Vector &) {}
        virtual void text_input(const char32_t *, size_t) {}
        virtual void key_down(const Keycode &) {}

        virtual void mouse_enter() {}       // TODO: provide "entry point" parameter ?
        virtual void mouse_exit() {}        // TODO: provide "exit point" parameter ?

        bool disabled() const { return false; } // TODO!!!

        /** Convention: the provided position is an offset to be added to the widget's
            own coordinates.
         */
        virtual void render(Canvas_t *, const Point &offset) = 0;

        virtual bool handle_key_down(const Keycode &) { return false; }

    protected:

        // Rendering conveniences

        // auto rgba_to_native(Canvas_t *, const Color &) -> Native_color;
        auto rgba_to_native(const Color &color) -> Native_color
        {
            return Canvas_t::rgba_to_native( color );
        }
        void fill_rect(Canvas_t *canvas, const Rectangle &rect, const Native_color &color)
        {
            canvas->fill_rect(rect.pos.x, rect.pos.y, rect.ext.w, rect.ext.h, color);
        }
        void fill_rect(Canvas_t *canvas, const Rectangle &rect, const Point &offs, const Native_color &color)
        {
            canvas->fill_rect(rect.pos.x + offs.x, rect.pos.y + offs.y, rect.ext.w, rect.ext.h, color);
        }
        void fill_rect(Canvas_t *canvas, const Point &pos, const Extents &ext, const Native_color &color)
        {
            canvas->fill_rect(pos.x, pos.y, ext.w, ext.h, color);
        }
        void fill(Canvas_t *canvas, const Point &offs, const Native_color &color)
        {
            fill_rect(canvas, rectangle(), offs, color);

            //auto b{ rectangle() };
            //r->fill_rect(offs.x + b.pos.x, offs.y + b.pos.y, b.ext.w, b.ext.h, color);
        }
        auto convert_position_to_inner(const Point &pos) -> Point
        {
            // TODO: subtract border and padding
            return pos;
        }
        auto advance_to_glyph_at(const Rasterized_font *font, const std::u32string &text, size_t from, size_t to, Point &pos) 
            -> const Glyph_control_box *
        {
            assert(to >= from);

            const Glyph_control_box *cbox = nullptr;

            for (auto i = from; ; i++)
            {
                cbox = & font->lookup_glyph(0, text[i])->cbox;
                if (i == to) break;
                pos.x += cbox->adv_x;
                pos.y += cbox->adv_y;
            }

            return cbox;
        }
        void draw_borders(Canvas_t *canvas, const Point & offs, Width width, const Color &color)
        {
            draw_borders(canvas, rectangle(), offs, width, color);
        }
        void draw_borders(Canvas_t *canvas, const Rectangle &rect, const Point &offs, Width width, const Color &color)
        {
            draw_borders(canvas, rect, offs, width, color, color, color, color);
        }
        void draw_borders(Canvas_t *canvas, const Rectangle &rect, const Point &offs, 
            Width width, const Color & top, const Color & right, const Color & bottom, const Color & left)
        {
            // TODO: this painting procedure does a "wrap" in clockwise fashion, without regard for corners
            fill_rect(canvas, offs + rect.pos + Point{(int) width, 0}, {rect.ext.w - width, width}, top);
            fill_rect(canvas, offs + rect.pos + Point{(int) (rect.ext.w - width), 0}, {width, rect.ext.h}, right);
            fill_rect(canvas, offs + rect.pos + Point{0, (int) (rect.ext.h - width)}, {rect.ext.w - width, width}, bottom);
            fill_rect(canvas, offs + rect.pos, {width, rect.ext.h}, left);
        }
        // PROVISIONAL
        void draw_stippled_inner_rect(Canvas_t *canvas, const Rectangle &, const Point &offs)
        {
            //canvas->draw_stipp()
        }

        // Experimental & temporary: implement more sophisticated (and flexible!) styling
        // - May not / should not stay static; make const if possible

        static auto stroke_width() -> int { return 1; }
        static auto stroke_color() -> Color { return { 0, 0, 0, 1 }; }
        //static auto padding() -> int { return 5; }
        static auto paper_color() -> Color { return {1, 1, 1, 1}; }

    private:

        Rectangle               _rect = {};
    };

    template <class Config, bool With_layout, class Parent> struct Widget__Layouter;

    // Widget 

    template <class Config, bool With_layout>
    class Widget: 
        public Config::template Widget_updater< 
            Widget__Layouter<Config, With_layout,
                Abstract_widget<Config, With_layout> > >
    {
    public:
        using Renderer = typename Config::Renderer;
        using Canvas_t = typename Widget::Canvas_t;
        using Font_handle = typename Renderer::font_handle;
        using Keyboard = typename Config::Keyboard;
        using Keycode = typename Keyboard::Keycode;
        using Abstract_container_t = Abstract_container<Config, With_layout>;
        using Root_widget_t = Root_widget<Config, With_layout>;
        using Click_handler = typename Abstract_widget::Click_handler;

        void set_background_color(const Color &color)
        {
            _bkgnd_clr = color;
        }
        auto background_color() const
        {
            // TODO: stylesheets!
            return _bkgnd_clr;
        }

        void on_click(Click_handler handler)
        {
            assert(!_click_hndlr);
            _click_hndlr = handler;
        }

        // void init();

        void set_visible(bool visible = true)
        {
            _visible = visible;
        }
        bool visible() const { return _visible; }

        void set_focussable(bool state = true) { _focussable = state; }
        bool focussable() const { return _focussable; } // TODO: replace with can_take_focus() that takes other factors into consideration ?

        void added_to_container(Abstract_container_t *cont)
        {
            _container = cont;
        }
        void removed_from_container(Abstract_container_t *)
        {
            _container = nullptr;
        }

        // TODO: should the following be protected ?
        bool hovered() const { return _hovered; }

        virtual void take_focus()
        {
            pass_up_and_notify_focus();
        }
        /** Important: do not call gained_focus() from a child; call child_obtained_focus() instead,
            to inform the container.
         */
        virtual void gained_focus ()
        {
            invalidate();
        }
        virtual void loosing_focus()
        {
            // container()->child_is_loosing_focus(); // TODO ?
            invalidate();
        }

        // TODO: rename to has_keyboard_focus() ?
        bool has_focus() { return container()->container_has_focus() && container()->focused_child() == this; }

        bool is_first_child() { return container()->children().front() == this; }
        bool is_last_child () { return container()->children().back () == this; }

        void key_down(const Keycode &key)
        {
            // Fallback behavior: try to handle locally, else let it "bubble up"

            if (!handle_key_down(key))
            {
                // Let the event "bubble" up
                container()->child_key_down(key);
            }
        }
        //void key_up(const Keycode &);
        void mouse_button(const Point &, int /*button*/, Key_state state) override
        {
            if (state == Key_state::pressed)
            {
                root_widget()->capture_mouse(this);
            }
            else if (state == Key_state::released)
            {
                root_widget()->release_mouse();
            }
        }

        void mouse_click(const Point &pos, int button, int count) override
        {
            take_focus();

            if (_click_hndlr) _click_hndlr(pos, button, count);
        }

        void mouse_enter() override
        {
            //std::cout << "Widget::mouse_enter()" << std::endl;
            _hovered = true;
            invalidate();
        }
        void mouse_exit() override
        {
            //std::cout << "Widget::mouse_exit()" << std::endl;
            _hovered = false;
            invalidate();
        }

        void change_visible(bool vis = true)
        {
            if (vis != visible())
            {
                set_visible(vis);
                invalidate();
            }
        }

    protected:

        auto container() const { return _container; }

        virtual auto root_widget() -> Root_widget_t * { return _container->container_root_widget(); }

        void pass_up_and_notify_focus() // default take_focus() action
        {
            if (!has_focus())
            {
                if (_focussable)
                {
                    container()->container_take_focus(this);
                    gained_focus();
                }
                else {
                    container()->container_take_focus(nullptr);
                }
            }
        }

        // Graphics system integration
        void shift_horizontally(Position_delta delta)
        {
            // TODO: 
            set_position({ position().x + delta, position().y });
            invalidate();
        }
        void shift_vertically(Position_delta delta)
        {
            // TODO: 
            set_position({ position().x, position().y + delta });
            invalidate();
        }
        void shift_up(Length length)
        {
            shift_vertically( - static_cast<Position_delta>(length) );
        }
        void shift_down(Length length)
        {
            shift_vertically( static_cast<Position_delta>(length) );
        }

        // Static styles
        // TODO: move to "stylesheet"
        static auto default_dialog_background_color() -> Color { return {0.6f, 0.6f, 0.6f, 1}; }

        // Styling
        // TODO: move to new class Abstract_button<> ?
        auto button_face_color() -> Color
        {
            if (hovered())
            {
                return {0.8f, 0.8f, 0.8f, 1};
            }
            else {
                return {0.7f, 0.7f, 0.7f, 1};
            }
        }
        auto button_border_color() -> Color
        {
            if (hovered())
            {
                return {0.3f, 0.3f, 0.3f, 1};
            }
            else {
                return {0.1f, 0.1f, 0.1f, 1};
            }
        }
        auto button_border_width() -> int
        {
            if (/*is_default()*/ false) 
            {
                return 2;
            }
            else {
                return 1;
            }
        }

        Abstract_container_t   *_container = nullptr;

    private:
        friend class Drag_controller;
        friend class Root_widget<Config, With_layout>;

        Color                   _bkgnd_clr = {0, 0, 0, 0};
        Click_handler           _click_hndlr;
        bool                    _visible = true;
        bool                    _focussable = true;
        bool                    _hovered = false;
    };

    // Default implementations for Updating_aspect

    /** The "Updater" aspect of a widget is responsible for making sure it gets
        redrawn when invalidate() has been called.

        The default implementation uses a pointer to parent to pass up redraw
        requests until they reach the root widget, which "handles" the request
        by passing it along to callback function.

        Because there is chance that a different implementation may not need
        to follow that route (e.g. by calling render() directly), the pointer
        to container may not be needed, so it is a member of this aspect
        implementation rather than of Widget<> itself.

        TODO: THIS IS PROVISIONAL. If it turns out, during further development, 
        that a pointer to container is needed for reasons that are not dependent
        on the Updater (or any other) aspect family, that pointer, and the
        methods associated with it, should be moved to the Widget<> stem class.
     */
    template<class Config, bool With_layout> class Abstract_container;
    template<class Config, bool With_layout, class Parent> class Default_container_updater;

    template<class Config, bool With_layout, class Parent>
    struct Default__Widget__Updater: public Parent
    {
        class Widget_t: public Widget<Config, true> { friend struct Default__Widget__Updater; };
        using Abstract_container_t = Abstract_container<Config, With_layout>;

        void invalidate()
        {
            //auto c = static_cast<Abstract_container<GUIConfig, With_layout>*>(this->container());
            //c->child_invalidated(static_cast<Widget_t*>(this));
            auto c = p()->container();
            c->child_invalidated(static_cast<Widget_t*>(this));
        }

    private:
        auto p() { return static_cast<Widget_t*>(static_cast<Widget<Config, true>*>(this)); }
    };

    // Layouting aspect

    /** TODO: rename to reflect the fact that this is abstract ?
     */
    template <class Config, class Parent> 
    struct Widget__Layouter<Config, true, Parent>: public Parent
    {
        /** It is up to the implementation to guarantee that any internal state/data
        needed for layouting (including computing/returning the get_minimal_size())
        is kept up to date.
        */

        // Layouter aspect contract

        virtual void init_layout() = 0;
        virtual auto get_minimal_size  () -> Extents = 0;
        virtual auto get_preferred_size() -> Extents { return get_minimal_size(); }
        virtual void layout() = 0;

        //void set_padding(Width);
        //void set_padding(const std::initializer_list<Width> &);

        void set_rectangle(const Point &nw, const Point &se)
        {
            p()->set_position(nw);
            p()->set_extents(Extents::between_points(nw, se));
        }
        void set_rectangle_nw(const Point &pos, const Extents &ext)
        {
            p()->set_position(pos);
            p()->set_extents(ext);
        }
        void set_rectangle_ne(const Point &pos, const Extents &ext)
        {
            p()->set_position({ pos.x - static_cast<Position>(ext.w), pos.y });
            p()->set_extents(ext);
        }
        void set_rectangle_se(const Point &pos, const Extents &ext)
        {
            p()->set_position({ pos.x - static_cast<Position>(ext.w), pos.y - static_cast<Position>(ext.h) });
            p()->set_extents(ext);
        }
        void set_rectangle_sw(const Point &pos, const Extents &ext)
        {
            p()->set_position({ pos.x, pos.y - static_cast<Position>(ext.h) });
            p()->set_extents(ext);
        }

    protected:

        class Widget_t: public Widget<Config, true> { friend struct Widget__Layouter; };
        auto p() { return static_cast<Widget_t*>(static_cast<Widget<Config, true>*>(this)); }

        // "Stylesheet" TODO: make this into another aspect ?
        static constexpr auto button_padding() -> Padding { return { 5, 5, 5, 5 }; }
    };

} // ns cppgui
