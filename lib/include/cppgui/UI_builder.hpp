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

        auto&& done()
        {
            return std::move(_widgets);
        }

    protected:

        void internal_add(Widget *w)
        {
            _widgets.push_back(std::unique_ptr<Widget>{w});
        }

        virtual auto container() -> Container_base & = 0;

    private:
        Widget_bag              _widgets;
        //const Rasterized_font  *_font = nullptr;
    };

    template<class ContainerT, class ParentT> class UI_builder;

    template<class ContainerT, class ParentT = void>
    class UI_builder_base2: public UI_builder_base
    {
    public:
        using Type = UI_builder<ContainerT, ParentT>;

        explicit UI_builder_base2(ContainerT &cont): 
            _cont{cont} 
        {}

        template<class WidgetT, typename... Args>
        auto add(Args &&... args) -> UI_builder<ContainerT, ParentT>&
        {
            auto w = new WidgetT{std::forward<Args>(args)...};
            internal_add(w);
            return *static_cast<Type*>(this);
        }

        /*
         * Begin a nested container.
         */
        template<class NestedT, typename... Args>
        auto begin(Args&&... args) -> UI_builder<NestedT, ContainerT>
        {
            auto c = new NestedT{std::forward<Args>(args)...};
            internal_add(c);
            return UI_builder<NestedT, ContainerT>{};
        }

        auto end() -> UI_builder<ContainerT, ParentT> &
        {
            return *static_cast<Type*>(this);
        }

    protected:

        auto container() -> ContainerT & override { return static_cast<ContainerT&>(_cont); }

    private:
        ContainerT         &_cont;
    };


    /* 
     * Unspecialized builder, to be specialized in header files of specific containers.
     */    
    template<class ContainerT, class ParentT> 
    class UI_builder: public UI_builder_base2<ContainerT, ParentT> 
    {};


    // Misc ---------------------------------------------------------

    template<class ContainerT>
    auto build_ui(ContainerT &cont) -> UI_builder<ContainerT, void>
    {
        return UI_builder<ContainerT, void>{cont};
    }

} // ns cppgui
