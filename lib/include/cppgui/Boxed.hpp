#pragma once

#include "./Box.hpp"


namespace cppgui {
 
    /*
     * Note: the Boxed<> decorator MUST NOT be used on abstract classes - for example, it is not 
     * possible to apply it to Container_base.
     * (The reason for this is that Boxed<> modifieds the two main layouting methods by overriding
     * and super-calling them.)
     */
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

        auto get_minimal_bounds() -> Bbox override
        {
            auto bbox = WidgetC::get_minimal_bounds();

            return this->adjust_box_bounds(bbox);
        }

        void compute_layout(Bbox_cref b)
        {
            WidgetC::compute_layout(this->adjust_box_bounds(b, -1));
        }

    #endif // !CPPGUI_EXCLUDE_LAYOUTING
    };

} // ns cppui