#include "./Scrollbox.hpp"

namespace cppgui {

    // Main class -----------------------------------------

    template<class Config, bool With_layout>
    void Scrollbox<Config, With_layout>::set_content(Container_t *content)
    {
        assert(!_content);

        _content = content;
        add_child(content);
    }

    // Layouter aspect ------------------------------------

    template<class Config>
    template<class Aspect_parent>
    void Scrollbox__Layouter<Config, true>::Aspect<Aspect_parent>::layout()
    {
        p()->_content->layout();
    }

} // ns cppgui