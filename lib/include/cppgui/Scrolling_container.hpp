#pragma once

#include "./Container.hpp"
#include "./Vertical_scrollbar.hpp"


namespace cppgui {
    
    class Content_pane: public Container_base
    {
    public:
        virtual auto get_minimal_window() -> Extents = 0;
    };


    class Scrolling_container: public Container_base
    {
    public:

        explicit Scrolling_container(Content_pane *);

        void compute_view_from_data() override;

    protected:

        bool handle_key_down(const Keycode &) override;

        auto content_window() const { return Rectangle{ _window_bbox }; }
        auto content_pane() const { return _content_pane; }
        auto items() const { return content_pane()->children(); }
        auto item(Index i) const { return content_pane()->child(i); }
        auto item_count() const -> Index { return content_pane()->child_count(); }

        void shift_content_by(const Point &);
        void update_scrollbar();

    private:
        using Parent = Container_base;

        virtual void navigate(Navigation_unit, const Fraction<int> &delta) = 0;

        Custom_vertical_scrollbar   _vscrollbar;
        Content_pane               *_content_pane;
        Bounding_box                _window_bbox;
        Point                       _content_offset;    // keeps track of content pane relative position


    #ifndef CPPGUI_EXCLUDE_LAYOUTING
    public:

        auto get_minimal_bounds() -> Bounding_box override;
        void compute_layout(const Bounding_box &) override;

    #endif // !CPPGUI_EXCLUDE_LAYOUTING
    };

} // ns cppgui