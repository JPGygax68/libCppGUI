#pragma once

#include <sstream>
#include <map>
#include <utility>
#include <cereal/archives/binary.hpp>
#include "./rasterized_font.hpp"
#include "./cereal.hpp"


namespace cppgui {
    
    namespace fonts {

        struct Store
        {
            const uint8_t  *data;
            size_t          size;

            // Initialize from fixed-sized array
            template<typename T> Store(T arr): data{&arr[0]}, size{sizeof(T)} {}
        };

        // TODO?: put this into its own header file "load.hpp" ?

        inline auto load(const Store &store)
        {
            Rasterized_font rastfont;
            
            std::stringstream is{ std::string{ reinterpret_cast<const char *>(store.data), store.size } };
            cereal::BinaryInputArchive archive(is);
            archive(rastfont);
            
            return rastfont;
        }

        inline auto get(const Store &store) -> Rasterized_font *
        {
            static std::map<const uint8_t *, std::unique_ptr<Rasterized_font>> map;

            return map.emplace(store.data, std::make_unique<Rasterized_font>(load(store))).first->second.get();
        }

    } // ns fonts
    
} // ns cppgui
