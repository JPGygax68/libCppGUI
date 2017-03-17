#pragma once

#include <sstream>
#include <map>
#include <utility>
#include <cereal/archives/binary.hpp>
#include "./Rasterized_font.hpp"
#include "./cereal.hpp"


namespace cppgui {
    
    namespace fonts {

        struct Store
        {
            const uint8_t  *data;
            size_t          size;

            // Initialize from fixed-sized array
            //template<typename T, int Size> Store(T(&arr)[Size]): data{&arr[0]}, size{Size} {}
            template<int Size> Store(const uint8_t(&arr)[Size]): data{&arr[0]}, size{Size} {}
        };

        // TODO?: put this into its own header file "load.hpp" ?

        inline auto load(const Store &store)
        {
            Rasterized_font rastfont;
            
            auto bs = std::string{ reinterpret_cast<const char *>(store.data), store.size };
            std::istringstream is{ bs, std::ios::binary };
            cereal::BinaryInputArchive archive{is};
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
