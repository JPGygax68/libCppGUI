#pragma once

#include <cppgui/Box_model.hpp>

//#pragma warning(disable: 4505)

namespace cppgui
{
    
    auto Box_model_base::box_rectangle() -> Rectangle
    {
        return {
            {
                box_inset(3),
                box_inset(0)
            },
            {
                extents().w - p()->box_inset(3) - p()->box_inset(1),
                extents().h - p()->box_inset(0) - p()->box_inset(2)
            }
        };
    }

    auto Box_model_base::content_rectangle() -> Rectangle
    {
        return {
            {
                p()->content_inset(3),
                p()->content_inset(0)
            },
            {
                this->extents().w - p()->content_inset(3) - p()->content_inset(1),
                this->extents().h - p()->content_inset(0) - p()->content_inset(2)
            }
        };
    }

    template <class Config, bool With_layout>
    template <class Impl, class Parent>
    auto Box_model_base<Config, With_layout>::Aspect<Impl, Parent>::position_text_element(const Text_bounding_box & bbox, Alignment minor_align, Alignment major_align) const -> Point
    {
        auto r = this->content_rectangle(); // this->extents() );
        Point pos;

        if (minor_align == Alignment::cultural_minor_start)
        {
            pos.x = r.pos.x;
        }
        else if (minor_align == Alignment::cultural_minor_middle)
        {
            pos.x = r.left() + (r.ext.w - bbox.width()) / 2;
        }
        else if (minor_align == Alignment::cultural_minor_end)
        {
            pos.x = r.right() - bbox.width();
        }

        if (major_align == Alignment::cultural_major_start)
        {
            pos.y = r.top() + bbox.y_max;
        }
        else if (major_align == Alignment::cultural_major_middle)
        {
            pos.y = r.top() + (r.ext.h - bbox.height()) / 2 + bbox.y_max;
        }
        else if (major_align == Alignment::cultural_major_end)
        {
            pos.y = r.bottom() + bbox.y_min;
        }
        else
        {
            assert(false);
            return {};
        }

        return pos;
    }

    template <class Config, bool With_layout>
    template <class Impl, class Parent>
    Box_model<Config, With_layout, Box_model_definition::run_time>::Aspect<Impl, Parent>::Aspect()
    {
        // The following will all be no-ops in case of build-time box model definition

        p()->set_margin(0, p()->default_margin(0));
        p()->set_border_width(0, p()->default_border_width(0));
        p()->set_padding(0, p()->default_padding(0));

        p()->set_margin(1, p()->default_margin(1));
        p()->set_border_width(1, p()->default_border_width(1));
        p()->set_padding(1, p()->default_padding(1));

        p()->set_margin(2, p()->default_margin(2));
        p()->set_border_width(2, p()->default_border_width(2));
        p()->set_padding(2, p()->default_padding(2));

        p()->set_margin(3, p()->default_margin(3));
        p()->set_border_width(3, p()->default_border_width(3));
        p()->set_padding(3, p()->default_padding(3));
    }

    template <class Config, bool With_layout>
    template <class Impl, class Parent>
    auto Box_model<Config, With_layout, Box_model_definition::run_time>::Aspect<Impl, Parent>::add_boxing(const Extents & ext) -> Extents
    {
        return {
            ext.w + this->content_inset(3) + this->content_inset(1),
            ext.h + this->content_inset(0) + this->content_inset(2)
        };
    }

    template <class Config, bool With_layout>
    template <class Impl, class Parent>
    void Box_model<Config, With_layout, Box_model_definition::run_time>::Aspect<Impl, Parent>::draw_border(Canvas_t * canvas, const Point & offset)
    {
        auto rect = this->box_rectangle();
        Width w;

        //bool ena = true; // this->enabled(); // TODO: implement enabled() in Widget<> !!
        //bool hov = this->hovered();
        //bool foc = this->has_focus();

        w = p()->border_width(0);
        this->fill_rect(canvas, {rect.pos + Point{w, 0}, {rect.ext.w - w, w}}, offset,
                        Canvas_t::rgba_to_native(p()->border_color(0)));
        w = p()->border_width(1);
        this->fill_rect(canvas, {rect.pos + Point{rect.ext.w - w, 0}, {w, rect.ext.h}}, offset,
                        Canvas_t::rgba_to_native(p()->border_color(1)));
        w = p()->border_width(2);
        this->fill_rect(canvas, {rect.pos + Point{0, rect.ext.h - w}, {rect.ext.w - w, w}}, offset,
                        Canvas_t::rgba_to_native(p()->border_color(2)));
        w = p()->border_width(3);
        this->fill_rect(canvas, {rect.pos, {w, rect.ext.h}}, offset,
                        Canvas_t::rgba_to_native(p()->border_color(2)));
    }
} // ns cppgui