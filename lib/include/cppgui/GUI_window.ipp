/*  libCppGUI - A GUI library for C++11/14
    
    Copyright 2016 Hans-Peter Gygax

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

#include <iostream> // TODO: for debugging only, remove!

#include "./GUI_window.hpp"

namespace cppgui {


    template<class Impl, class GUIConfig, class WindowBaseT, template <class> class RendererAdapter>
    GUI_window<Impl, GUIConfig, WindowBaseT, RendererAdapter>::GUI_window(const char *title, int w, int h)
    {
    }

    /** Caution! the following is called from the constructor of the concrete class derived from
        GUI_window<>.

        TODO: better name for this method ?
    */
    template<class Impl, class GUIConfig, class WindowBaseT, template <class> class RendererAdapter>
    inline void GUI_window<Impl, GUIConfig, WindowBaseT, RendererAdapter>::init_window()
    {
    }

    template<class Impl, class GUIConfig, class WindowBaseT, template <class> class  RendererAdapter>
    inline void GUI_window<Impl, GUIConfig, WindowBaseT, RendererAdapter>::cleanup_window()
    {
    }

    /*
    template<class Impl, class GUIConfig, class WindowBaseT, template <class> class RendererAdapter>
    inline void GUI_window<Impl, GUIConfig, WindowBaseT, RendererAdapter>::init_gui()
    {
        if (!_init_done)
        {
            _root_widget.init();
            _init_done = true;
        }
    }
    */

    template<class Impl, class GUIConfig, class WindowBaseT, template <class> class  RendererAdapter>
    inline void GUI_window<Impl, GUIConfig, WindowBaseT, RendererAdapter>::redraw()
    {
    }

    template<class Impl, class GUIConfig, class WindowBaseT, template <class> class  RendererAdapter>
    inline void GUI_window<Impl, GUIConfig, WindowBaseT, RendererAdapter>::size_changed(int w, int h)
    {
    }

    template<class Impl, class GUIConfig, class WindowBaseT, template <class> class  RendererAdapter>
    void GUI_window<Impl, GUIConfig, WindowBaseT, RendererAdapter>::mouse_motion(int x, int y)
    {
    }

    template<class Impl, class GUIConfig, class WindowBaseT, template <class> class  RendererAdapter>
    void GUI_window<Impl, GUIConfig, WindowBaseT, RendererAdapter>::mouse_button(int x, int y, int button, int dir, int count)
    {
    }

    template<class Impl, class GUIConfig, class WindowBaseT, template <class> class  RendererAdapter>
    void GUI_window<Impl, GUIConfig, WindowBaseT, RendererAdapter>::mouse_wheel(int x, int y)
    {
    }

    template<class Impl, class GUIConfig, class WindowBaseT, template <class> class  RendererAdapter>
    void GUI_window<Impl, GUIConfig, WindowBaseT, RendererAdapter>::text_input(const char32_t *text, size_t size)
    {
    }

    template<class Impl, class GUIConfig, class WindowBaseT, template <class> class  RendererAdapter>
    void GUI_window<Impl, GUIConfig, WindowBaseT, RendererAdapter>::key_down(const Keycode &key)
    {
    }

    template<class Impl, class GUIConfig, class WindowBaseT, template <class> class  RendererAdapter>
    void GUI_window<Impl, GUIConfig, WindowBaseT, RendererAdapter>::closing()
    {
    }

} // ns cppgui