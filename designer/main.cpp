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

#include <iostream>
#include <memory>

#include <SDL_main.h>

#include <cppgui/sdl/Application.hpp>

#include "./Main_window.hpp"

int main(int /*argc*/, char * /*argv*/ [])
{
    try
    {
        auto win = std::make_unique<Main_window>();
        cppgui::sdl::Application<Main_window> app;

        app.run();

        return 0;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        std::cout << "Press RETURN to terminate" << std::endl;
        char ch; std::cin >> std::noskipws >> ch;
        return -1;
    }
}