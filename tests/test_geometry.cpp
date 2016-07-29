#include <iostream>
#include <string>

#include <cppgui/oriented_geometry.hpp>

using namespace cppgui;

static void test_Oriented_position()
{
    std::cout << "Oriented_position<> (both variants)" << std::endl << std::endl;

    // TODO: because of a supposed Visual C++ 2015 bug, Oriented_position<> cannot be fully checked with static_assert().

    static_assert( Oriented_position<false>{ 5 } == 5, "Oriented_position<false>: construct from Position" );
    //static_assert( Oriented_position<false>{ 5 } + 10 == 15, "Oriented_position<false>: ctor + operator +" );
    // .. we do this instead
    std::cout << "  Oriented_position<Reversed = false>::operator +: ... ";
    if (Oriented_position<false>{ 5 } + 10 != 15) throw std::runtime_error("Oriented_position<false>::operator + failure");
    std::cout << "OK" << std::endl;

    std::cout << "  Oriented_position<Reversed = true>::operator +: ... ";
    if (Oriented_position<true>{ 5 } + 10 != -5) throw std::runtime_error("Oriented_position<true>::operator + failure");
    std::cout << "OK" << std::endl;

    std::cout << std::endl;
}

static void test_Oriented_point()
{
    using namespace std::literals::string_literals;

    std::cout << "Oriented_point<>" << std::endl << std::endl;

    {
        Oriented_point<left_to_right, top_down, false> pt;
        pt.longitude() = 5;
        pt.longitude() += 10;
        if (pt.longitude() != 15) throw std::runtime_error("Oriented_point<left_to_right, top_down, false>: longitude not adding correctly");
        pt.latitude() = 7;
        pt.latitude() += 10;
        if (pt.latitude() != 17) throw std::runtime_error("Oriented_point<left_to_right, top_down, false>: latitude not adding correctly");
    }

    {
        Oriented_point<left_to_right, top_down, true> pt;
        pt.longitude() = 5;
        pt.longitude() += 10;
        if (pt.longitude() != 15) throw std::runtime_error("Oriented_point<left_to_right, top_down, true>: longitude not adding correctly");
        pt.latitude() = 7;
        pt.latitude() += 10;
        if (pt.latitude() != -3) throw std::runtime_error("Oriented_point<left_to_right, top_down, true>: latitude not adding correctly");
    }

    {
        Oriented_point<right_to_left, top_down, false> pt;
        pt.longitude() = 5;
        pt.longitude() += 10;
        if (pt.longitude() != -5) throw std::runtime_error("Oriented_point<right_to_left, top_down, false>: longitude not adding correctly");
        pt.latitude() = 7;
        pt.latitude() += 10;
        if (pt.latitude() != 17) throw std::runtime_error("Oriented_point<right_to_left, top_down, false>: latitude not adding correctly");
    }

    {
        Oriented_point<right_to_left, top_down, true> pt;
        pt.longitude() = 5;
        pt.longitude() += 10;
        if (pt.longitude() != -5) throw std::runtime_error("Oriented_point<right_to_left, top_down, true>: longitude not adding correctly");
        pt.latitude() = 7;
        pt.latitude() += 10;
        if (pt.latitude() != -3) throw std::runtime_error("Oriented_point<right_to_left, top_down, true>: latitude not adding correctly");
    }

    {
        Oriented_point<right_to_left, bottom_up, false> pt;
        pt.longitude() = 5;
        pt.longitude() += 10;
        if (pt.longitude() != -5) throw std::runtime_error("Oriented_point<right_to_left, bottom_up, false>: longitude not adding correctly");
        pt.latitude() = 7;
        pt.latitude() += 10;
        if (pt.latitude() != -3) throw std::runtime_error("Oriented_point<right_to_left, bottom_up, false>: latitude not adding correctly");
    }

    {
        Oriented_point<right_to_left, bottom_up, true> pt;
        pt.longitude() = 5;
        pt.longitude() += 10;
        if (pt.longitude() != -5) throw std::runtime_error("Oriented_point<right_to_left, bottom_up, true>: longitude not adding correctly");
        pt.latitude() = 7;
        pt.latitude() += 10;
        if (pt.latitude() != 17) throw std::runtime_error("Oriented_point<right_to_left, bottom_up, true>: latitude not adding correctly");
    }

}


template<Orientation LonOrient, Orientation LatOrient, bool YAxisBottomUp>
static void test_Oriented_rectangle_instance()
{
    static constexpr Oriented_rectangle<LonOrient, LatOrient, YAxisBottomUp> c_rect { 10, 15, 30, 20 };
    static auto rect { c_rect };

    // TODO: Visual Studio 2015 bug: cannot make this constexpr
    static auto pt = c_rect.position();
    static_assert( std::is_same<Oriented_point<LonOrient, LatOrient, YAxisBottomUp>, decltype(pt)>::value, "wrong type");
    auto& pt2 = c_rect.position();
    static_assert( std::is_same<const Oriented_point<LonOrient, LatOrient, YAxisBottomUp> &, decltype(pt2)>::value, "wrong type");
    auto& pt3 = rect.position();
    static_assert( std::is_same<Oriented_point<LonOrient, LatOrient, YAxisBottomUp> &, decltype(pt3)>::value, "wrong type");
}

//template<Orientation Longitudinal, Orientation Latitudinal, bool YAxisBottomUp>
static void test_Oriented_rectangle()
{
    //std::cout << "Oriented_rectangle<Longitudinal = " << Longitudinal << ", Latitudinal = " << Latitudinal << ", YAxisBottomUp = " 
    //    << YAxisBottomUp << ">" << std::endl << std::endl;
    std::cout << "Oriented_rectangle<>" << std::endl << std::endl;

    {
        test_Oriented_rectangle_instance<left_to_right, top_down , false>();
        test_Oriented_rectangle_instance<left_to_right, top_down , true >();
        test_Oriented_rectangle_instance<right_to_left, top_down , false>();
        test_Oriented_rectangle_instance<right_to_left, top_down , true >();
        test_Oriented_rectangle_instance<left_to_right, bottom_up, false>();
        test_Oriented_rectangle_instance<left_to_right, bottom_up, true >();
        test_Oriented_rectangle_instance<right_to_left, bottom_up, false>();
        test_Oriented_rectangle_instance<right_to_left, bottom_up, true >();
    }

    //rect.set_longitudinal_segment( 10, 20 );
}

int main(int /*argc*/, char ** /*argv*/)
{
    static_assert( Axis_for_orientation<left_to_right>::value == horizontal, "Axis_for_orientation<left_to_right>::value == horizontal" );
    static_assert( Axis_for_orientation<right_to_left>::value == horizontal, "Axis_for_orientation<right_to_left>::value == horizontal" );
    static_assert( Axis_for_orientation<top_down     >::value == vertical  , "Axis_for_orientation<top_down     >::value == vertical  " );
    static_assert( Axis_for_orientation<bottom_up    >::value == vertical  , "Axis_for_orientation<bottom_up    >::value == vertical  " );

    test_Oriented_position();

    //test_Oriented_point<left_to_right, bottom_up, false>();
    test_Oriented_point();

    test_Oriented_rectangle();

    std::cout << "Press RETURN to terminate" << std::endl;
    char ch; std::cin >> std::noskipws >> ch;

    return -1;
}