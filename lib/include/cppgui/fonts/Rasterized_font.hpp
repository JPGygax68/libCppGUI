#pragma once

#include <vector>
#include <cstdint>
#include <algorithm>

#include "Rasterized_glyph_cbox.hpp"
#include "Character_range.hpp"
#include "Bounding_box.hpp"

namespace cppgui {

    namespace fonts {

        /*
            A Rasterized Font struct contains just what its name says: a set of glyphs, 
            rasterized into one or more grayscale bitmaps, plus the information needed to
            access those glyphs.

            This includes an index making it easy to find the glyph box corresponding to a given 
            Unicode code point (provided of course that code point is contained in the set).

            The struct is intended to be easy to serialize and de-serialize, containing 
            nothing but trivial or STL types.

            TODO: split up into glyph lookup table and pixel data
        */

        struct Rasterized_font {
        public:

            //Rasterized_font() = default;
            //Rasterized_font(Rasterized_font &&) = default;
            //Rasterized_font(const Rasterized_font &) = default;

            struct glyph_record {
                Rasterized_glyph_cbox cbox;
                size_t pixel_base;      // offset in pixel buffer where this glyph starts
            };
            
            // Contains the data specific to a given size/style combination
            struct variant {
                std::vector<uint8_t> pixels;
                std::vector<glyph_record> glyphs;
            };

            std::vector<Character_range>    ranges;
            std::vector<variant>            variants;

            auto find_glyph(uint32_t cp) const -> int {

                size_t base = 0; // base glyph index for current range
                for (auto range_it = begin(ranges); range_it != end(ranges); base += (*range_it).count, ++range_it)
                    if (cp >= (*range_it).starting_codepoint && cp < ((*range_it).starting_codepoint + (*range_it).count))
                        return base + (cp - (*range_it).starting_codepoint);
                return -1;
            }
            
            auto get_glyph(int variant, int index) const -> const glyph_record * {

                return & variants[variant].glyphs[index];
            }

            auto lookup_glyph(int variant, uint32_t cp) const -> const glyph_record * {

                return get_glyph(variant, find_glyph(cp));
            }

            // TODO: make this capable of working with vertical scripts
            
            auto compute_text_extents(int variant, const char32_t *text, size_t count) const -> Bounding_box {

                // TODO: the implementation should be inherited from (or forwarded to) a generic routine

                int x_min = 0, x_max = 0, y_min = 0, y_max = 0;

                if (count > 0) {
                    auto glyph = get_glyph(variant, find_glyph(*text));
                    x_min = glyph->cbox.bounds.x_min;
                    size_t i = 0;
                    int x = 0;
                    while (true) {
                        auto &cbox = glyph->cbox;
                        y_min = std::min(y_min, cbox.bounds.y_min);
                        y_max = std::max(y_max, cbox.bounds.y_max);
                        if (++i == count) break;
                        x += glyph->cbox.adv_x;
                        glyph = get_glyph(variant, find_glyph(text[i]));
                    }
                    x_max = x + glyph->cbox.bounds.x_max;
                }

                return { x_min, x_max, y_min, y_max };
            }

        };

    } // ns fonts
    
} // ns cppgui
