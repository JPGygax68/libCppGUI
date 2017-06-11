#pragma once

#include <typeindex>
#include <map>

// TODO: because of the definition of a non-namespaced macro, move this to an "internal" subdirectory ?


namespace cppgui {


    /*
     * The Parent_map class is a very simple type registry that can be used to determine, at runtime,
     * whether a class is the same as or a descendant of another class.
     * It is intended for widget styling, so that styles that are attributed to specific widgets can
     * also apply to their descendants.
     * 
     * To use, insert a line similar to the following somewhere at the top of a widget's source file:
     * 
     *  Parent_map::Registrant<WidgetClass, ParentClass> parent_registrant;
     */
    class Parent_map {
    public:

        class type_index_ext: public std::type_index
        {
        public:
            using type_index::type_index;

            type_index_ext(): type_index{typeid(void)} {}
        };

        template<class T>
        static void register_root()
        {
            map()[std::type_index(typeid(T))] = type_index_ext(typeid(void));
        }
        
        template<class T, class P>
        static void register_class()
        {
            map()[std::type_index(typeid(T))] = type_index_ext(typeid(P));
        }
        
        static bool check_ancestry(std::type_index a, std::type_index b)
        {
            return a == b || check_ancestry(map()[a], b);
        }
        
        template<class T, class P = void>
        struct Registrant
        {
            Registrant() { register_class<T, P>(); }
        };

        template<class T>
        struct Registrant<T, void>
        {
            Registrant() { register_root<T>(); }
        };

    private:
    
        using map_t = std::map<std::type_index, type_index_ext>;

        static auto map() -> map_t & { 
            static map_t m; return m; }
    };
    

    #define REGISTER_PARENTHOOD(a, b) static cppgui::Parent_map::Registrant<a, b> parent_map_registrant;
    #define REGISTER_ANCESTOR(a) static cppgui::Parent_map::Registrant<a> parent_map_registrant;

} // ns cppgui