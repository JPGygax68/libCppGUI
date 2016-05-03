#pragma once

#include "./Widget.hpp"
//#include "./List_pane.hpp"
#include "./Vertical_scrollbar.hpp"

namespace cppgui {

    // Forward declarations

    template<class Config, bool With_layout, class Pane> struct Scrollbox__Layouter { template<class Aspect_parent> struct Aspect: Aspect_parent {}; };
    template<class Config, bool With_layout> class Scrollable_pane;

    // Class definition

    // TODO: rename to Scrollbox_base ?

    template<class Config, bool With_layout, class Pane = Scrollable_pane<Config, With_layout>>
    class Scrollbox: public Scrollbox__Layouter<Config, With_layout, Pane>::template Aspect< Container<Config, With_layout> >
    {
    public:
        using Container_t = Container<Config, With_layout>;
        using Parent_t = Container_t;
        using Canvas_t = typename Canvas<typename Config::Renderer>;
        using Scrollable_pane_t = Pane; // Scrollable_pane<Config, With_layout>;
        using Keyboard = typename Config::Keyboard;
        using Keycode = typename Keyboard::Keycode;
        //using List_pane_t = List_pane<Config, With_layout>;

        using Navigation_handler = std::function<void(Navigation_unit)>;

        Scrollbox();

        void set_border(const Border &);

        void init() override;

        void on_navigation(Navigation_handler);

        void set_content_pane(Scrollable_pane_t *);
        auto content_pane() { return _content; }

        auto& vertical_scrollbar() { return _vert_sbar; }

        void mouse_wheel(const Vector &) override;
        void key_down(const Keycode &) override;

        void render(Canvas_t *, const Point &offset) override;

        auto content_rectangle() const -> const Rectangle & { return _content_rect; } // to be accessed by Scrollable_pane (or derived)

    protected:
        using Vertical_scrollbar_t = Custom_vertical_scrollbar<Config, With_layout>;

        auto pane() { return static_cast<Scrollable_pane_t*>(_content); }

        Border                  _border    = { 4, {0, 0.2f, 0.6f, 1} };     // encompasses both content area and scrollbar(s)
        Separator               _separator = { 3, {0, 0.4f, 0.8f, 1} };
        Vertical_scrollbar_t    _vert_sbar;
        Rectangle               _content_rect;      // set by layouter
        Position                _vert_sep_pos;      // positions of the separators (vertical = x, horizontal = y)
        Navigation_handler      _on_navigation;
        Scrollable_pane_t      *_content = nullptr;
    };

    // Layouter aspect

    template<class Config, class Pane>
    struct Scrollbox__Layouter<Config, true, Pane> {

        template<class Aspect_parent> struct Aspect: Aspect_parent {

            struct Scrollbox_t: public Scrollbox<Config, true, Pane> { friend struct Aspect; };
            auto p() { return static_cast<Scrollbox_t*>(this); }

            auto get_minimal_size() -> Extents override;
            void layout() override;

        protected:
            //auto content_rect() -> Rectangle;
        };
    };

    // Scrollable pane ==============================================

    template<class Config, bool With_layout>
    class Scrollable_pane: public Container<Config, With_layout>
    {
    public:
        //using Navigation_handler = Custom_vertical_scrollbar<Config, With_layout>;

        // To implement in CRTP descendant class:

        void scroll(Navigation_unit unit, Position initial_pos, Fraction<int> delta) { static_assert(false, "Scrollable_pane::navigate()"); }

        /** TODO: inform container scrollbox that the extents of the pane have changed,
            and to update the scrollbar(s) accordingly
         */
        void notify_extents_changed();
    };

} // ns cppgui