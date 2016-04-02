#include <cppgui/Icon_resources.hpp>

namespace cppgui {

    template <Font_size Size = 72>
    static constexpr auto get_font(Font_size size) -> const Rasterized_font *;

    constexpr auto Icon_resources::tick(Font_size size) -> Font_icon
    {
        return {
            get_font<>(size)
        };
    }

    template<Font_size Size>
    static constexpr auto get_font(Font_size size) -> const Rasterized_font *
    {
        static_assert(false, "Pre-rasterized font not available for request size");

        return {};
    }

} // ns cppgui