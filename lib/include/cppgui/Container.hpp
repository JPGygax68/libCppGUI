#pragma once

#include "./Widget.hpp"
#include "./Abstract_container.hpp"

namespace cppgui {

    // Container base class: descended from Widget

    enum class Layout_type { none, 
        header_content, 
        content_footer, 
        head_content,           // horizontal: fixed-width head, followed by stretching content TODO: better name! 
        content_tail,           // opposite of head_content
        end
    };

    // Forward-declare layouter aspect

    template <class Config, bool With_layout>
    struct Container_Layouter {

        template <class Aspect_parent> struct Aspect: public Aspect_parent {};
    };

    template <class Config, bool With_layout>
    class Container: 
        public Container_Layouter<Config, With_layout>::template Aspect < Widget<Config, With_layout> >, 
        public Config::template Container_Container_updater< Abstract_container<Config, With_layout> >
    {
    public:
        using Renderer = typename Config::Renderer;
        using Widget_t = typename Widget<Config, With_layout>;
        using Canvas_t = typename Widget_t::Canvas_t;
        using Abstract_container_t = typename Abstract_container<Config, With_layout>;

        void init() override;

        using Abstract_container_t::add_child;

        void mouse_motion(const Position &) override;
        void mouse_button(const Position &, int button, Key_state) override;
        void mouse_click(const Position &, int button, int count) override;
        void mouse_wheel(const Position &) override;

        void mouse_exit() override;

        //void update_render_resources(Renderer *) override;
        void render(Canvas_t *, const Position &) override;

    protected:
        //auto background_color() -> Color { return {1, 1, 0.7f, 1}; }
        auto paper_margin() -> unsigned int { return 2; }
    };

    template <class Config, bool With_layout> struct Default_Container_Container_updater {

        template <class Aspect_parent> struct Aspect: public Aspect_parent {

            class Container_t: public Container<Config, true> { friend struct Aspect; };
            using Widget_t = Widget<Config, true>;

            auto p() { return static_cast<Container_t*>(this); }

            void child_invalidated(Widget_t *) override;

            auto container_root_widget() { return p()->root_widget(); }
        };
    };

    // Layouter aspect

    template <class Config>
    struct Container_Layouter<Config, true> {

        template <class Aspect_parent> struct Aspect : public Aspect_parent {

            using Widget_t = typename Widget<Config, true>;
            class Container_t: public Container<Config, true> { friend struct Aspect; };

            auto p() { return static_cast<Container_t*>(this); }

            // Layout contract

            void init_layout() override;
            auto get_minimal_size() -> Extents override { return _comp_min_size; }
            void layout() override;

            // Specific interface

            void set_layout_type(Layout_type type ) { _layout_type = type; }
            void recalc_minimal_size();

            void insert_child(Widget_t *);
            void drop_child(Widget_t *);

            void set_spacing(Length spacing) { _spacing = spacing; }

        private:
            Layout_type     _layout_type = Layout_type::none;
            Length          _spacing = 0;
            Extents         _comp_min_size = { 0, 0 };
        };
    };

    // Nil implementation of the Layouter aspect

    template <class Config>
    struct Container_Layouter<Config, false> {

        template <class Aspect_parent> struct Aspect : public Aspect_parent {

            void recalc_minimal_size() {}
        };
    };
        
} // ns cppgui