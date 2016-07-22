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

    /*
    Oriented_rectangle<Config1, left_to_right> rect;

    std::cout << "Default value for Config::Renderer::y_axis_bottom_up : " << _vertical_axis_bottom_up<Config1>::value << std::endl;
    std::cout << "Explicit value for Config::Renderer::y_axis_bottom_up: " << _vertical_axis_bottom_up<Config2>::value << std::endl;
    */

    std::cout << "Oriented_rectangle: left-to-right" << std::endl;

    Oriented_rectangle<Config1, left_to_right> r1;
    r1.set_longitude_segment(10, 20);
    r1.set_latitude_segment ( 5, 10);
    if (r1.left  () != 10) throw std::runtime_error("left edge wrong"  );
    if (r1.right () != 30) throw std::runtime_error("right edge wrong" );
    if (r1.top   () !=  5) throw std::runtime_error("top edge wrong"   );
    if (r1.bottom() != 15) throw std::runtime_error("bottom edge wrong");

    std::cout << "Oriented_rectangle: right-to-left" << std::endl;

    Oriented_rectangle<Config1, right_to_left> r2;
    r2.set_longitude_segment(10, 20);
    r2.set_latitude_segment ( 5, 10);
    if (r2.left  () != -10) throw std::runtime_error("left edge wrong"  );
    if (r2.right () !=  10) throw std::runtime_error("right edge wrong" );
    if (r2.top   () !=   5) throw std::runtime_error("top edge wrong"   );
    if (r2.bottom() !=  15) throw std::runtime_error("bottom edge wrong");

    std::cout << "Oriented_rectangle: top-bottom, with downward (default) Y axis" << std::endl;

    Oriented_rectangle<Config1, top_down> r3;
    r3.set_longitude_segment(10, 20);
    r3.set_latitude_segment ( 5, 10);
    if (r3.left  () !=  5) throw std::runtime_error("left edge wrong"  );
    if (r3.right () != 15) throw std::runtime_error("right edge wrong" );
    if (r3.top   () != 10) throw std::runtime_error("top edge wrong"   );
    if (r3.bottom() != 30) throw std::runtime_error("bottom edge wrong");

    std::cout << "Oriented_rectangle: top-bottom, with upward Y axis" << std::endl;

    Oriented_rectangle<Config2, top_down> r4;
    r4.set_longitude_segment(10, 20);
    r4.set_latitude_segment ( 5, 10);
    if (r4.left  () !=   5) throw std::runtime_error("left edge wrong"  );
    if (r4.right () !=  15) throw std::runtime_error("right edge wrong" );
    if (r4.top   () !=  10) throw std::runtime_error("top edge wrong"   );
    if (r4.bottom() != -10) throw std::runtime_error("bottom edge wrong");



    std::cout << "Press RETURN to terminate" << std::endl;
    char ch; std::cin >> std::noskipws >> ch;

    return -1;
}