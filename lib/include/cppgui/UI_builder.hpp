#pragma once

#include <vector>
#include <memory>
#include <cassert>
#include "Widget.hpp"


namespace cppgui
{
    
    class Widget;
    class Container_base;


    class UI_builder_base
    {
    public:
        explicit UI_builder_base(Widget_bag &bag): _widgets{bag} {}

        /* auto&& done()
        {
            return std::move(_widgets);
        } */

    protected:

        void add_to_bag(Widget *w)
        {
            _widgets.push_back(std::unique_ptr<Widget>{w});
        }

        //virtual auto widget() -> Container_base & = 0;

    private:
        Widget_bag             &_widgets;
        //const Rasterized_font  *_font = nullptr;
    };

    template<class ContainerT, class ParentBuilderT> class UI_builder;

    /*
     * Template for a UI_builder for a specified container type, with an optional
     * parent container type.
     */
    template<class ContainerT, class ParentBuilderT>
    class UI_builder_base2: public UI_builder_base
    {
    public:
        using Type = UI_builder<ContainerT, ParentBuilderT>;

        explicit UI_builder_base2(ContainerT &cont, Widget_bag &bag, ParentBuilderT *pb):
            UI_builder_base{bag},
            _cont{cont},
            _bag{bag},
            _parent_builder{pb}
        {}

        /*
         * Default implementation of _create_new_child<>() template method.
         */
        template<class WidgetT, typename... Args>
        auto _create_new_child(Args &&... args) -> WidgetT *
        {
            return new WidgetT{std::forward<Args>(args)...};
        }

        template<class WidgetT>
        void _add_new_child(Widget *)
        {
            static_assert(false, "UI_builder<> specialization must implement _add_new_child<>()");
        }

        /*
         * Add a child to the container represented by this builder.
         */
        template<class NestedT, typename... Args>
        auto add(Args&&... args) -> Type &
        {
            NestedT *child = static_cast<Type*>(this)->template _create_new_child<NestedT>(std::forward<Args>(args)...);
            static_cast<Type*>(this)->_add_new_child(child);
            add_to_bag(child);            
            return *static_cast<Type*>(this);
        }

        /*
         * Begin work on a new nested container: create it, add it to the current level container,
         * and return a builder instance for the nested container.
         */
        template<class NestedT, typename... Args>
        auto begin(Args&&... args) -> UI_builder<NestedT, Type>
        {
            NestedT *child = static_cast<Type*>(this)->template _create_new_child<NestedT>(std::forward<Args>(args)...);
            static_cast<Type*>(this)->_add_new_child(child);
            add_to_bag(child);            
            // Create and return a builder for the child
            NestedT &child_ref = *child;
            return UI_builder<NestedT, Type>{child_ref, this->_bag, static_cast<Type*>(this)};
        }

        template<typename T=ParentBuilderT>
        auto end() -> typename std::enable_if<!std::is_same<T, void>::value, T&>::type
        {
            return *_parent_builder;
        }

    protected:

        auto widget() -> ContainerT & { return _cont; }

    private:
        ContainerT         &_cont;
        Widget_bag         &_bag;
        ParentBuilderT     *_parent_builder = nullptr;
    };


    /* 
     * Unspecialized builder, to be specialized in header files of specific containers.
     */    
    //template<class ContainerT, class ParentBuilderT> class UI_builder; //: public UI_builder_base2<ContainerT, ParentBuilderT> {};

    /*
     * "Root" builder: creates and "carries" the widget bag.
     */

    template<class ContainerT> 
    class Root_UI_builder: public UI_builder_base2<ContainerT, void>
    {
    public:
        explicit Root_UI_builder(ContainerT &cont, Widget_bag &bag): 
            UI_builder_base2<ContainerT, void>{cont, bag, nullptr} 
        {}
    };

    // Misc ---------------------------------------------------------

    template<class ContainerT>
    auto build_ui(ContainerT &cont, Widget_bag &bag)
    {
        return std::move(Root_UI_builder<ContainerT>{cont, bag});
    }

} // ns cppgui
