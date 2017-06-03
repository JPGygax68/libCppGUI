#pragma once

#include <algorithm>
#include <vector>
#include <memory>
#include <cassert>
#include "Widget.hpp"
#include "Text_widget.hpp"
#include "Grid_container.hpp"


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


    template<class ContainerT> class UI_builder;

    template<class ContainerT>
    class UI_builder_base2: public UI_builder_base
    {
    public:
        explicit UI_builder_base2(ContainerT &cont): 
            _cont{cont} 
        {}

        template<class WidgetT, typename... Args>
        auto add(Args &&... args) -> UI_builder<ContainerT>&
        {
            auto w = new WidgetT{std::forward<Args>(args)...};
            internal_add(w);
            return static_cast<UI_builder<ContainerT>&>(*this);
        }

    protected:

        auto container() -> Container_base & override { return _cont; }

    private:
        ContainerT         &_cont;
    };

    
    // Specilizations -----------------------------------------------

    // TODO: move to header declaring container each container type ?

    template<class ContainerT> class UI_builder: public UI_builder_base2<ContainerT> {};

    template<>
    class UI_builder<Grid_container>: public UI_builder_base2<Grid_container>
    {
    public:
        using UI_builder_base2::UI_builder_base2;

        template<class WidgetT, typename... Args>
        auto add(Args &&... args) -> UI_builder<Grid_container>&
        {
            UI_builder_base2::add<WidgetT>(std::forward<Args>(args)...);
            _col_index ++;
            return *this;
        }

        auto& end_row()
        {
            _row_index ++;
            _col_index = 0;
            return *this;
        }

    private:
        int _row_index = 0;
        int _col_index = 0;
    };


    // Misc ---------------------------------------------------------

    template<class ContainerT>
    auto build_ui(ContainerT &cont) -> UI_builder<ContainerT>
    {
        UI_builder<ContainerT> builder{cont};

        return std::move(builder);
    }

} // ns cppgui
