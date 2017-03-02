#pragma once

#include <sstream>
#include <map>
#include <utility>

#include <cereal/archives/binary.hpp>

#include "./rasterized_font.hpp"
#include "./cereal.hpp"

namespace cppgui {
    
    namespace fonts {

        // TODO: replace with GSL span<> ?

        using Data_store = std::pair<const uint8_t *, std::size_t>;

        /* struct Data_store {
            const uint8_t *data;
            std::size_t    size;
        }; */

        // TODO?: put this into its own header file "load.hpp" ?

        inline auto load(const Data_store &store)
        {
            Rasterized_font rastfont;
            
            std::stringstream is(std::string{ reinterpret_cast<const char *>(store.first), store.second });
            cereal::BinaryInputArchive archive(is);
            archive(rastfont);
            
            return rastfont;
        }

        inline auto get(const Data_store &store) -> Rasterized_font *
        {
            static std::map<const uint8_t *, std::unique_ptr<Rasterized_font>> map;

            return map.emplace(store.first, std::make_unique<Rasterized_font>(load(store))).first->second.get();
        }

        // FOR COMPATIBILITY

        inline auto load(const uint8_t *data, size_t size) { return load({data, size}); }

        inline auto get(const uint8_t *data, size_t size) { return get({data, size}); }

    } // ns fonts
    
} // ns cppgui
