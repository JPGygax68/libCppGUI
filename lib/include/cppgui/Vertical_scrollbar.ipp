#include "./Vertical_scrollbar.hpp"

namespace cppgui {

    template<class Config, bool With_layout>
    Vertical_scrollbar<Config, With_layout>::Vertical_scrollbar()
    {
        _up_btn  .set_glyph(cppgui::Icon_resources<24>::up_arrow  ());
        _down_btn.set_glyph(cppgui::Icon_resources<24>::down_arrow());

        add_child(&_up_btn  );
        add_child(&_down_btn);
    }

    // Layouter aspect ----------------------------------------------

    template<class Config>
    template<class Aspect_parent>
    void Vertical_scrollbar__Layouter<Config, true>::Aspect<Aspect_parent>::layout()
    {
        auto ext = p()->extents();

        auto minsz_up_btn   = p()->_up_btn.get_minimal_size();
        auto minsz_down_btn = p()->_down_btn.get_minimal_size();

        p()->_up_btn  .set_rectangle_nw({0, 0           }, { ext.w, minsz_up_btn  .h} );
        p()->_down_btn.set_rectangle_sw({0, ext.bottom()}, { ext.w, minsz_down_btn.h} );

        p()->_up_btn  .layout();
        p()->_down_btn.layout();
    }


} // ns cppgui