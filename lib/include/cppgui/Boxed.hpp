#pragma once

#include "./Box.hpp"


namespace cppgui {
 
    template<class WidgetC, class BoxStyles>
    class Boxed: public WidgetC, public Box<BoxStyles>
    {
    public:
        void render(Canvas *c, const Point &offset) override
        {
            auto p = offset + this->position();

            draw_background_and_border(c, p, this->bounds(), this->visual_states());

            WidgetC::render(c, offset);
        }

    #ifndef CPPGUI_EXCLUDE_LAYOUTING
    public:

        //void init_layout() override;

        auto get_minimal_bounds() -> Bounding_box override
        {
            auto bbox = WidgetC::get_minimal_bounds();

            return this->adjust_box_bounds(bbox);
        }

        void compute_layout(const Bounding_box &b)
        {
            WidgetC::compute_layout(this->adjust_box_bounds(b, -1));
        }

    #endif // CPPGUI_EXCLUDE_LAYOUTING
    };

} // ns cppui