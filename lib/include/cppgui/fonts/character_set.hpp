#pragma once

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

#include <cassert>
#include <algorithm>
#include "./character_range.hpp"


namespace cppgui {

    namespace fonts {
    
        class Character_set {
        public:

            auto add(uint32_t start, uint32_t count = 1) -> Character_set &
            {
                /** TODO: implement an internal version that returns a reference to the _ranges
                iterator where the added range has been stored. This will make it easier
                to implement an efficient merge function.
                */
                using namespace std;
                
                // Find where to insert the new range
                auto it = begin(_ranges);
                while (it != end(_ranges) && start > (it->starting_codepoint + it->count)) it++;
                
                // After last current range ?
                if (it == end(_ranges)) 
                {
                    _ranges.emplace_back<Character_range>({start, count});
                }
                // No: beginning before or inside an existing range, or appending to one
                else {
                    uint32_t first = min(it->starting_codepoint, start);

                    // Find last of the successors we'll replace
                    auto it2 = it;
                    for (auto itn = it2; ++itn != end(_ranges) && (start + count) >= itn->starting_codepoint; it2 = itn);

                    // Compute last (+1: non-inclusive) character in new range
                    uint32_t last = max(start + count, it2->starting_codepoint + it2->count);

                    // Erase the ranges we're replacing, except the first
                    _ranges.erase(it + 1, it2 + 1);

                    // Re-use the existing range                    
                    *it = { first, last - first };
                }

                return *this;
            }

            auto add(const Character_range &range) -> Character_set &
            {
                return add(range.starting_codepoint, range.count);
            }

            /** The remove() method is intended to scratch off characters whose glyphs cannot
                be found. As this should be the exception, removing a range is not supported
                here.
             */
            auto remove(uint32_t ch) -> Character_set & 
            {
                using namespace std;

                for (auto it = begin(_ranges); it != end(_ranges); it++) 
                {
                    if (it->contains(ch)) 
                    {
                        if (it->starting_codepoint == ch) {
                            it->starting_codepoint++;
                            it->count--;
                        }
                        else if (ch == it->starting_codepoint + it->count - 1) 
                        {
                            it->count--;
                        }
                        else {
                            uint32_t offset = ch - it->starting_codepoint;
                            uint32_t remaining = it->count - offset;
                            it->count = offset;
                            _ranges.emplace<Character_range>(it + 1, { it->starting_codepoint + offset + 1, remaining });
                        }
                    }
                    break; // we're done
                }
                return *this;
            }

            bool contains(uint32_t ch) const 
            {
                using namespace std;

                return any_of(begin(_ranges), end(_ranges), [&](auto range) { return range.contains(ch); });
            }

            auto& ranges() const { return _ranges; }

        private:

            std::vector<Character_range> _ranges;
        };
        
    } // ns fonts
    
} // ns cppgui