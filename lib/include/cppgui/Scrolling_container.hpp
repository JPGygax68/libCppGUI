#pragma once

#include "./Container.hpp"
#include "./Vertical_scrollbar.hpp"


namespace cppgui {
    
    class Scrolling_container;


    class Content_pane: public Container_base
    {
    public:

        bool focussable() const override { return false; }

        virtual auto get_minimal_window() -> Extents = 0;

    protected:
        auto container() const -> Scrolling_container *;
    };


    /*
     * TODO: update the hovered states after the content has been scrolled - perhaps
     *  send a dummy mouse_move() event to itself ?
     *  
     * TODO: automatic scrollbar(s) with adapting content pane
     */
    class Scrolling_container: public Container_base
    {
    public:

        explicit Scrolling_container(Content_pane *);

        void compute_view_from_data() override;

        void render(Canvas *, const Point &offs) override;

        bool key_down(const Keycode &) override;
        bool mouse_wheel(const Vector &) override;

        bool cycle_focus(int) override { return false; } // prevent from cycling away from content pane

        void bring_item_into_view(Index);
        void bring_item_into_view(Widget *);

        auto content_pane() const { return _content_pane; }

        auto& items() const { return content_pane()->children(); }
        auto item(Index i) const { return content_pane()->child(i); }
        auto item_count() const -> Index { return content_pane()->child_count(); }
        auto item_index(Widget *c) const { return content_pane()->child_index(c); }
        auto item_rectangle(Widget *) const -> Rectangle; 

    protected:

        auto content_window() const { return Rectangle{ _window_bbox }; }

        //void gained_focus() override;

        void shift_content_by(const Point &);
        void update_scrollbar();

    private:
        using Super = Container_base;

        virtual void navigate(Navigation_unit, const Fraction<int> &d) = 0;

        auto separator_color(Widget_states) const -> RGBA;
        static auto separator_width() { return 1; }

        Embedded_vertical_scrollbar _vscrollbar;
        Content_pane               *_content_pane;
        Bbox                        _window_bbox;
        Point                       _content_offset;    // keeps track of content pane relative position


    #ifndef CPPGUI_EXCLUDE_LAYOUTING
    public:

        auto get_minimal_bounds() -> Bbox override;
        void compute_layout(Bbox_cref) override;
        auto get_optimal_bounds() -> Bbox override;

    #endif // !CPPGUI_EXCLUDE_LAYOUTING
    };

} // ns cppgui