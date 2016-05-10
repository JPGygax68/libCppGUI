#pragma once

#include "./Widget.hpp"
#include "./Box.hpp"
#include "./Vertical_scrollbar.hpp"

namespace cppgui {

    // Forward declarations

    template<class Config, bool With_layout, class Pane> struct Scrollbox__Layouter { template<class Aspect_parent> struct Aspect: Aspect_parent {}; };
    template<class Config, bool With_layout> class Scrollable_pane;

    // Class definition

    // TODO: rename to Scrollbox_base ?

    template<class Config, bool With_layout, class Pane> // = Scrollable_pane<Config, With_layout>>
    class Scrollbox: public Scrollbox__Layouter<Config, With_layout, Pane>::template Aspect< Container<Config, With_layout> >,
        public Bordered_box<Scrollbox<Config, With_layout, Pane>>
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

    // Forward declarations

    template<class Config, bool With_layout> struct Scrollable_pane__Layouter { template<class Aspect_parent> struct Aspect: Aspect_parent {}; };

    // Base class

    template<class Config, bool With_layout>
    class Scrollable_pane_base: public Container<Config, With_layout>
    {
    public:
        //using Navigation_handler = Custom_vertical_scrollbar<Config, With_layout>;

        // To implement in CRTP descendant class:

        // TODO: the interface should probably be reduced to the single scroll() method; however initial_pos 
        //      should be handled by the scrollbar
        void scroll(Navigation_unit unit, /* Position initial_pos, */ Fraction<int> delta) { static_assert(false, "Scrollable_pane::navigate()"); }
        void scroll_by_items(int delta) { static_assert(false, "Scrollable_pane::scroll_by_items()"); };
        void scroll_by_pages(int delta) { static_assert(false, "Scrollable_pane::scroll_by_pages()"); };

        /** TODO: inform container scrollbox that the extents of the pane have changed,
            and to update the scrollbar(s) accordingly
         */
        void notify_extents_changed();
    };

    // Main class template

    template<class Config, bool With_layout>
    class Scrollable_pane: public Scrollable_pane__Layouter<Config, With_layout>::template Aspect< Scrollable_pane_base<Config, With_layout> >
    {
    };

    // Layouter aspect

    template<class Config>
    struct Scrollable_pane__Layouter<Config, true> {

        template<class Aspect_parent> struct Aspect: Aspect_parent {

            struct Scrollable_pane_t: public Scrollable_pane<Config, true> { friend struct Aspect; };
            auto p() { return static_cast<Scrollable_pane_t*>(this); }

            void compute_and_set_extents(const Extents &container_extents) { static_assert(false, "CRPT"); }

            //auto get_minimal_size() -> Extents override;
            //void layout() override;
        };
    };

} // ns cppgui

#define CPPGUI_INSTANTIATE_SCROLLBOX(Config, With_layout, PaneType) \
    template cppgui::Scrollbox          <Config, With_layout, PaneType>; \
    template cppgui::Scrollbox__Layouter<Config, With_layout, PaneType>;
    //template cppgui::Scrollable_pane<Config, With_layout>;
