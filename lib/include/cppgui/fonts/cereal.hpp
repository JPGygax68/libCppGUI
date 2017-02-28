#pragma once

#include <cereal/types/vector.hpp>

#include "./rasterized_font.hpp"

namespace cereal {

	template <class Archive>
	void serialize(Archive & archive, cppgui::fonts::Character_range &range)
	{
		archive(range.starting_codepoint, range.count);
	}

	template <class Archive>
	void serialize(Archive & archive, cppgui::fonts::Rasterized_font::glyph_record &glrec)
	{
		archive(glrec.cbox, glrec.pixel_base);
	}

	template <class Archive>
	void serialize(Archive & archive, cppgui::fonts::Rasterized_font::variant &var)
	{
		archive(var.glyphs, var.pixels);
	}

    template <class Archive>
    void serialize(Archive & archive, cppgui::fonts::Bounding_box &box)
    {
        archive(box.x_min, box.x_max, box.y_min, box.y_max);
    }

    template <class Archive>
    void serialize(Archive & archive, cppgui::fonts::Rasterized_glyph_cbox &cbox)
    {
        archive(cbox.bounds);
        archive(cbox.adv_x);
        archive(cbox.adv_y);
    }

	template <class Archive>
	void serialize(Archive & archive, cppgui::fonts::Rasterized_font &rfont)
	{
		archive(rfont.ranges, rfont.variants);
	}

} // ns cereal