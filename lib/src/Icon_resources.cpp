#include <Icon_resources.hpp>


namespace cppgui {
    
    // TODO: use a CMake-generated preprocessor symbol instead of hardcoded array name
    auto Icon_resources::bell() -> Glyph_specifier { return {fonts::get({modernpics_24}), 0x39}; }

    auto Icon_resources::up_arrow() -> Glyph_specifier { return {fonts::get({modernpics_24}), 0x5D}; }

    auto Icon_resources::down_arrow() -> Glyph_specifier { return {fonts::get({modernpics_24}), 0x5B}; }

} // ns cppgui