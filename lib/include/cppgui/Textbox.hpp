#pragma once

#include "./Widget.hpp"

namespace cppgui {

    template <class Config>
    struct Textbox_layouter {

        CPPGUI_DEFINE_ASPECT(Aspect)
        {
            auto minimal_size()->Extents override;
            void layout() override;
        };
    };

    template <class Config, bool With_layout>
    class Textbox: 
        public select_aspect<With_layout, Textbox_layouter<Config>::Aspect, Nil_layouter>::template aspect<Widget<Config, With_layout>>
    {

    };

} // ns cppgui
