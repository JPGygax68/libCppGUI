/*
    The MIT License(MIT)

    Copyright(c) 2014 Jean-Pierre Gygax, Biel/Bienne, Switzerland

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files(the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions :

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
    THE SOFTWARE.
*/

#include <iostream>
#include <stdexcept>
#include <string>
#include <cstdint>
#include <set>

#include <boost/filesystem.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include FT_ERRORS_H

#include <cereal/archives/binary.hpp>

#include <gpc/fonts/rasterized_glyph_cbox.hpp>
#include <gpc/fonts/character_range.hpp>
#include <gpc/fonts/character_set.hpp>
#include <gpc/fonts/rasterized_font.hpp>
#include <gpc/fonts/cereal.hpp>

typedef std::pair<std::string, std::string> NameValuePair;

static auto splitParam(const std::string &param) -> NameValuePair {

    auto p = param.find_first_of('=');
    std::string name, value;

    if (p != std::string::npos) {
        name = param.substr(0, p);
        value = param.substr(p + 1);
    }
    else {
        name = param;
    }

    return std::move( NameValuePair(name, value) );
}

static auto findFontFile(const std::string &file) -> std::string {

    using std::string;
    using std::runtime_error;
    using namespace boost::filesystem;

    path file_path(file);

    // If the unaltered path leads to an existing file, there's nothing to do
    if (exists(file_path)) return file_path.string();

    // If the path is not absolute, try OS-dependent prefixes
    if (!file_path.is_absolute()) {
#ifdef _WIN32
        const char *system_root = getenv("SystemRoot");
        if (system_root == nullptr) system_root = "C:\\Windows";
        path full_path = path(system_root) / "Fonts" / file_path;
        if (!exists(full_path)) throw runtime_error(string("Couldn't find font file \"") + file + "\" at any known location");
        return full_path.string();
#else
#pragma warn "Font file search paths not yet implemented for any platform except MS Windows (Tm)"
#endif
    }
    
    throw runtime_error(string("Font file \"") + file + "\" doesn't exist");
}

int main(int argc, const char *argv[])
{
    using namespace std;
	using gpc::fonts::rasterized_glyph_cbox;
    using gpc::fonts::character_range;
	using gpc::fonts::rasterized_font;

    int exit_code = -1;

    try {

        string font_file, output_file;
		set<uint16_t> sizes;
        gpc::fonts::character_set char_set;
        bool full_range = false;

        // Get command-line arguments
        for (auto i = 1; i < argc; i ++) {
            auto name_value = splitParam(argv[i]);
			auto &name = name_value.first, &value = name_value.second;
            if (name == "input") {
                //cout << "file = " << name_value.second << endl;
				//cout << "font file path = " << font_file << endl;
				font_file = findFontFile(value);
            }
			else if (name == "size") {
				sizes.insert((int16_t)stoi(value));
			}
			else if (name == "output") {
				output_file = value;
			}
            else if (name == "range")
            {
                size_t j = 0, k;
                for (; ;)
                {
                    k = value.find(',');
                    auto v = value.substr(j, k == std::string::npos ? k : k - j);
                    if (v == "all" || v == "full" || v == "complete")
                    {
                        char_set.add(1, 0x10ffff);
                        full_range = true;
                        std::cout << "Including complete Unicode rage (disabling warnings for missing glyphs)" << std::endl;
                    }
                    else if (v[0] == '$')
                    {
                        auto cp = std::stoi(v.substr(1), nullptr, 16);
                        char_set.add(cp, 1);
                    }

                    if (k == std::string::npos) break;

                    j = k + 1;
                }
            }
            else {
                throw runtime_error(string("invalid parameter \"") + argv[i] + "\"");
            }
        }

        if (font_file.empty()) throw runtime_error("No font file specified!");
		if (sizes.empty()) throw runtime_error("No sizes specified!");
		if (output_file.empty()) throw runtime_error("No output file specified!");

        if (char_set.ranges().empty()) char_set.add(32, 255); // extended Latin or whatever

        FT_Library library;
        FT_Error fterror;

        fterror = FT_Init_FreeType(&library);
        if (fterror) throw runtime_error("FreeType library failed to initialize");

        FT_Face face;
        fterror = FT_New_Face(library, font_file.c_str(), 0, &face);
        if (fterror) throw runtime_error("Couldn't load or use font file");

        cout << "Font file contains " << face->num_faces << " face(s)." << endl;
        cout << "This face contains " << face->num_glyphs << " glyphs." << endl;
		cout << "Face flags:";
		if ((face->face_flags & FT_FACE_FLAG_SCALABLE        ) != 0) cout << " scalable";
		if ((face->face_flags & FT_FACE_FLAG_FIXED_SIZES     ) != 0) cout << " fixed sizes";
		if ((face->face_flags & FT_FACE_FLAG_FIXED_WIDTH     ) != 0) cout << " fixed width";
		if ((face->face_flags & FT_FACE_FLAG_SFNT            ) != 0) cout << " SFNT";
		if ((face->face_flags & FT_FACE_FLAG_HORIZONTAL      ) != 0) cout << " horizontal";
		if ((face->face_flags & FT_FACE_FLAG_VERTICAL        ) != 0) cout << " vertical";
		if ((face->face_flags & FT_FACE_FLAG_KERNING         ) != 0) cout << " kerning";
		if ((face->face_flags & FT_FACE_FLAG_FAST_GLYPHS     ) != 0) cout << " fast_glyphs";
		if ((face->face_flags & FT_FACE_FLAG_MULTIPLE_MASTERS) != 0) cout << " multiple_masters";
		if ((face->face_flags & FT_FACE_FLAG_GLYPH_NAMES     ) != 0) cout << " glyph_names";
		if ((face->face_flags & FT_FACE_FLAG_EXTERNAL_STREAM ) != 0) cout << " external stream";
		if ((face->face_flags & FT_FACE_FLAG_HINTER          ) != 0) cout << " hinter";
		if ((face->face_flags & FT_FACE_FLAG_CID_KEYED       ) != 0) cout << " CID_keyed";
		if ((face->face_flags & FT_FACE_FLAG_TRICKY          ) != 0) cout << " tricky";
		if ((face->face_flags & FT_FACE_FLAG_COLOR           ) != 0) cout << " color";
		cout << endl;
		cout << "Style flags: ";
		if ((face->style_flags & FT_STYLE_FLAG_BOLD          ) != 0) cout << " bold";
		if ((face->style_flags & FT_STYLE_FLAG_ITALIC        ) != 0) cout << " italic";
		cout << endl;
        if ((face->face_flags & FT_FACE_FLAG_SCALABLE)) cout << "This font is scalable" << endl;
        cout << "Units per EM: " << face->units_per_EM << endl;
        cout << "Number of fixed sizes: " << face->num_fixed_sizes << endl;

        //fterror = FT_Set_Char_Size(face, 0, 16*64, 300, 300);
        //if (fterror) throw runtime_error("Failed to set character size (in 64ths of point)");

		rasterized_font rast_font;
		rast_font.variants.resize(sizes.size()); // TODO: support styles, not just sizes

		uint32_t glyph_count = 0, missing_count = 0;
		uint32_t next_codepoint = 0;

        // Find out what glyphs can be found in this font file

        for (auto &range: char_set.ranges())
        {
		    for (uint32_t cp = range.starting_codepoint; cp <= range.starting_codepoint + range.count; cp++)
            {
			    FT_UInt glyph_index = FT_Get_Char_Index(face, cp);

			    if (glyph_index > 0)
                {
				    // Add this codepoint to the range, or begin a new range
				    if (cp > next_codepoint) rast_font.ranges.emplace_back<character_range>({ cp, 0 });
				    character_range &out_range = rast_font.ranges.back();
				    out_range.count++;
				    next_codepoint = cp + 1;
				    glyph_count++;
			    }
			    else {
				    if (!full_range) cout << "No glyph for codepoint " << cp << endl;
				    missing_count++;
			    }
		    }
        }

		cout << endl;
		cout << "Total number of glyphs: " << glyph_count << endl;
		cout << "Number of char ranges:  " << rast_font.ranges.size() << endl;
		cout << "Missing codepoints:     " << missing_count << endl;

		// Repeat for each pixel size
		
		int max_ascender = 0;
		int max_descender = 0;
		auto i_size = 0;

		for (auto size : sizes)
        {
			for (auto const &range : rast_font.ranges)
            {
                for (size_t i = 0; i < range.count; i++)
                {
                    // Codepoint
                    uint32_t cp = range.starting_codepoint + i;

                    // Get glyph index (again)
                    FT_UInt glyph_index = FT_Get_Char_Index(face, cp);

                    // Select font size (in pixels)
                    fterror = FT_Set_Pixel_Sizes(face, 0, size);
                    if (fterror) throw runtime_error("Failed to set character size (in pixels)");

                    fterror = FT_Load_Glyph(face, glyph_index, FT_LOAD_DEFAULT);
                    if (fterror) throw runtime_error("Failed to get glyph");

                    fterror = FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);
                    if (fterror) throw runtime_error("Failed to render loaded glyph");

                    FT_GlyphSlot slot = face->glyph;
                    FT_Bitmap &bitmap = slot->bitmap;

                    assert((slot->advance.x & 0x3f) == 0);

                    auto &variant = rast_font.variants[i_size];
                    auto &pixels = variant.pixels;

                    int ascender  = slot->bitmap_top;
                    int descender = slot->bitmap.rows - slot->bitmap_top;
                    if (ascender  > max_ascender ) max_ascender  = ascender ;
                    if (descender > max_descender) max_descender = descender;

                    variant.glyphs.emplace_back(rasterized_font::glyph_record {
                        slot->bitmap_left, static_cast<signed>(bitmap.width) + slot->bitmap_left,
                        slot->bitmap_top - static_cast<signed>(bitmap.rows), slot->bitmap_top,
                        slot->advance.x >> 6, slot->advance.y >> 6,
                        variant.pixels.size()
                    });

					/*
					auto &cbox = variant.glyphs.back().cbox;
					cout << "Codepoint: " << cp << ", " << "glyph: " << glyph_index << " " << "at size " << size << ": "
						 << "width: "     << cbox.width << ", " << "height: "  << cbox.rows  << ", "
						 << "left: "      << cbox.left  << ", " << "top: "     << cbox.top   << ", "
						 << "adv_x : "    << cbox.adv_x << ", " << "adv_y: "   << cbox.adv_y << ", "
						 << "ascent: "    << ascender   << ", " << "descent: " << descender
						 << endl;
					*/

					// Copy the pixels
					uint32_t pixel_base = pixels.size();
					pixels.resize(pixel_base + bitmap.width * bitmap.rows);
					auto dit = pixels.begin() + pixel_base;
					auto sit = bitmap.buffer;
                    for (auto j = 0U; j < bitmap.rows; j++, sit += bitmap.pitch)
                    {
                        for (auto k = 0U; k < bitmap.width; k++) *dit++ = sit[k];
                    }

				} // each character in the range

		    } // each range

			cout << endl;
			cout << "Size " << size << ":" << endl;
			cout << "Total number of pixels: " << rast_font.variants[i_size].pixels.size() << endl; // TODO: multiple variants
			cout << "Max ascender:           " << max_ascender  << endl;
			cout << "Max descender:          " << max_descender << endl;

			// Next size
			i_size++;

		} // each pixel size

		// Serialize the result to the output file
		ofstream os(output_file, ios_base::binary);
        cereal::BinaryOutputArchive archive(os);
        archive(rast_font);

        cout << "Done." << endl;

		exit_code = 0;
	}
    catch(exception &e) {
        cerr << "Error: " << e.what() << endl;
    }

	/* cout << endl;
    cout << "Press RETURN to terminate" << endl;
    cin.ignore(); */

    return exit_code;
}