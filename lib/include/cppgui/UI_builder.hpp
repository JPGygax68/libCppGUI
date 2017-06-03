#pragma once

#include <algorithm>
#include <vector>
#include <memory>
#include <cassert>
#include "Widget.hpp"
#include "Text_widget.hpp"


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

        void internal_add(Widget *w);

        virtual auto container() -> Container_base & = 0;

    private:
        Widget_bag              _widgets;
        const Rasterized_font  *_font = nullptr;
    };


    template<class ContainerT>
    class UI_builder: public UI_builder_base
    {
    public:
        explicit UI_builder(ContainerT &cont): 
            _cont{cont} 
        {}

        template<class WidgetT, typename... Args>
        auto add(Args &&... args) -> UI_builder&
        {
            auto w = new WidgetT{std::forward<Args>(args)...};
            internal_add(w);
            return *this;
        }

    protected:

        auto container() -> ContainerT & override { return _cont; }

    private:
        ContainerT         &_cont;
    };

    template<class ContainerT>
    auto build_ui(ContainerT &cont) -> UI_builder<ContainerT>
    {
        UI_builder<ContainerT> builder{cont};

        return std::move(builder);
    }

} // ns cppgui
