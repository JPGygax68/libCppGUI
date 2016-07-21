#include <iostream>

#include <cppgui/Rectangle.hpp>

struct Dummy_renderer {};

struct Config1
{
    using Renderer = Dummy_renderer;
};

struct Dummy_renderer_2
{
    static constexpr bool y_axis_bottom_up = true;
};

struct Config2
{
    using Renderer = Dummy_renderer_2;
};

int main(int /*argc*/, char * /*argv*/[])
{
    using namespace cppgui;

    Oriented_rectangle<Config1, left_to_right> rect;

    std::cout << "Default value for Config::Renderer::y_axis_bottom_up : " << _vertical_axis_bottom_up<Config1>::value << std::endl;
    std::cout << "Explicit value for Config::Renderer::y_axis_bottom_up: " << _vertical_axis_bottom_up<Config2>::value << std::endl;


    std::cout << "Press RETURN to terminate" << std::endl;
    char ch; std::cin >> std::noskipws >> ch;

    return -1;
}