#include <iostream>
#include <string>

template <class WidgetT, class Updater, class Config>
struct _Widget: public Updater {
    
    void invalidate();
    
    int widget_data = 100;
};

template <class WidgetT>
struct Default_widget_updater {
    
    virtual void do_invalidate() { std::cout << "widget_data = " << static_cast<WidgetT*>(this)->widget_data << std::endl; }
};

template <class Config> struct Container;

template <class ContainerT>
struct Default_container_updater: public Default_widget_updater<ContainerT> {
    
    void do_invalidate() override { 
        std::cout 
            << "widget_data = " << static_cast<ContainerT*>(this)->widget_data << ", " 
            << "container_data = " << static_cast<ContainerT*>(this)->container_data 
            << std::endl; 
    }
};

template <class Config>
struct Container: public _Widget<Container<Config>, typename Config::Container_updater, Config> {
    
    int container_data = 200;
};

//-----------------------

template <class WidgetT, class Updater, class Config>
void _Widget<WidgetT, Updater, Config>::invalidate()
{
    Updater::do_invalidate();
}

//-----------------------

template <class Config>
struct My_widget: public _Widget<My_widget<Config>, typename Config::Widget_updater, Config> {
    
};

struct My_config {
    using Widget_updater    = Default_widget_updater   <My_widget<My_config>>;
    using Container_updater = Default_container_updater<Container<My_config>>;
};

int main()
{
    My_widget<My_config> w;
    Container<My_config> c;
    
    w.invalidate();
    c.invalidate();
    
    return 0;
}