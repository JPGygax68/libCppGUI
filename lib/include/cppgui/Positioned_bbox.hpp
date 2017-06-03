#pragma once

#include "./Bbox.hpp"


namespace cppgui
{

    struct Positioned_bbox;
    using Pbbox = Positioned_bbox;
    using Pbbox_ref = Pbbox &;
    using Pbbox_cref = const Pbbox &;

    struct Positioned_bbox
    {
        Point   orig;
        Bbox    bbox;

        auto left  () const { return orig.x + bbox.x_min; }
        auto center() const { return orig.x + bbox.x_min + bbox.width() / 2; }
        auto right () const { return orig.x + bbox.x_max; }
        auto top   () const { return orig.y - bbox.y_max; }
        auto middle() const { return orig.y - bbox.y_max + bbox.height() / 2; }
        auto bottom() const { return orig.y - bbox.y_min; }

        auto& set_left_edge_to  (Position x) { orig.x = x - bbox.x_min; return *this; }
        auto& set_origin_to     (Position x) { orig.x = x; return *this; }
        auto& set_center_to     (Position x) { orig.x = x - bbox.width() / 2 - bbox.x_min; return *this; }
        auto& set_right_edge_to (Position x) { orig.x = x - bbox.x_max; return *this; }
        auto& set_top_edge_to   (Position y) { orig.y = y - bbox.y_max; return *this; }
        auto& set_baseline_to   (Position y) { orig.y = y; return *this; }
        auto& set_middle_to     (Position y) { orig.y = y - bbox.height() / 2 + bbox.y_max; return *this; }
        auto& set_bottom_to     (Position y) { orig.y = y + bbox.y_min; return *this; }

        auto& align_origin_to  (Pbbox_cref ref) { orig.x = ref.orig.x; return *this; }
        auto& align_left_to    (Pbbox_cref ref) { orig.x = ref.left  () - bbox.x_min; return *this; }
        auto& align_right_to   (Pbbox_cref ref) { orig.x = ref.right () - bbox.x_max; return *this; }
        auto& align_center_to  (Pbbox_cref ref) { orig.x = ref.center() - bbox.width() / 2 - bbox.x_min; return *this; }
        auto& align_baseline_to(Pbbox_cref ref) { orig.y = ref.orig.y; return *this; }
        auto& align_top_to     (Pbbox_cref ref) { orig.y = ref.top   () + bbox.y_max; return *this; }
        auto& align_bottom_to  (Pbbox_cref ref) { orig.y = ref.bottom() + bbox.y_min; return *this; }
        auto& align_middle_to  (Pbbox_cref ref) { orig.y = ref.middle() - bbox.height() / 2 + bbox.y_max; return *this; }
    };

} // ns cppgui