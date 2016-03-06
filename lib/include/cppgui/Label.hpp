#pragma once

#include <string>

#include "./Widget.hpp"

namespace gpc { namespace fonts {

    struct rasterized_font;
} }

namespace cppgui {

    template <class Renderer>
    class Label : public Widget<Renderer> {
    public:
        void set_font(const gpc::fonts::rasterized_font *);
        void set_text(const std::u32string &);

        auto minimal_size() -> Extents override;
        void render(Renderer *) override;

        struct Layouter {
            void update(Label &);
        };

    private:
        const gpc::fonts::rasterized_font  *_font;
        std::u32string                      _text;
    };

} // ns cppgui