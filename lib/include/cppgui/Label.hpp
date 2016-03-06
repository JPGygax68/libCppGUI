#pragma once

#include <string>

#include "./Widget.hpp"

namespace gpc { namespace fonts {

    struct rasterized_font;
} }

namespace cppgui {

    template <class Renderer>
    class Label : public Widget<Label<Renderer>, Renderer> {
    public:
        void set_font(const gpc::fonts::rasterized_font *);
        void set_text(const std::u32string &);

        auto minimal_size() -> Extents override;

        void update_resources(Renderer *) override;
        void render(Renderer *, const Position &offset) override;

        struct Layouter {
            static void update(Label &);
        };

    private:
        const gpc::fonts::rasterized_font  *_font;
        std::u32string                      _text;
        Position                            _txpos;
        Font_handle                         _fnthnd;
    };

} // ns cppgui