#pragma once

#include "./Container.hpp"
#include "./Vertical_scrollbar.hpp"


namespace cppgui {
    
    class Content_pane: public Container_base
    {
        
    };


    class Scrolling_container: public Container_base
    {
    public:

        explicit Scrolling_container(Content_pane *);

    protected:

        auto& content_rectangle() const { return _content_rect; }
        auto content_pane() const { return _content_pane; }
        auto items() const { return content_pane()->children(); }

    private:

        virtual void navigate(Navigation_unit, const Fraction<int> &delta) = 0;

        Custom_vertical_scrollbar   _vscrollbar;
        Content_pane               *_content_pane;
        Rectangle                   _content_rect;

    #ifndef CPPGUI_EXCLUDE_LAYOUTING
    public:

        auto get_minimal_bounds() -> Bounding_box override;
        void compute_layout(const Bounding_box &) override;

    #endif // !CPPGUI_EXCLUDE_LAYOUTING
    };

} // ns cppgui