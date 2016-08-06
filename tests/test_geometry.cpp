#include <iostream>
#include <string>

#include <cppgui/oriented_geometry.hpp>

using namespace cppgui;

static void test_Oriented_position()
{
    std::cout << "Oriented_position<> (both variants)" << std::endl << std::endl;

    // TODO: because of a supposed Visual C++ 2015 bug, Oriented_position<> cannot be fully checked with static_assert().

    static_assert( Directed_position<false>{ 5 } == 5, "Oriented_position<false>: construct from Position" );
    //static_assert( Oriented_position<false>{ 5 } + 10 == 15, "Oriented_position<false>: ctor + operator +" );
    // .. we do this instead
    std::cout << "  Oriented_position<Reversed = false>::operator +: ... ";
    if (Directed_position<false>{ 5 } + 10 != 15) throw std::runtime_error("Oriented_position<false>::operator + failure");
    std::cout << "OK" << std::endl;

    std::cout << "  Oriented_position<Reversed = true>::operator +: ... ";
    if (Directed_position<true>{ 5 } + 10 != -5) throw std::runtime_error("Oriented_position<true>::operator + failure");
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

static void test_Oriented_extents()
{
    using namespace std::literals::string_literals;

    std::cout << "Oriented_extents<>" << std::endl << std::endl;

    {
        Oriented_extents<left_to_right, top_down, false> ext;
        ext.length() = 5;
        ext.length() += 10;
        if (ext.length() != 15) throw std::runtime_error("Oriented_extents<left_to_right, top_down, false>: length not adding correctly");
        ext.breadth() = 7;
        ext.breadth() += 10;
        if (ext.breadth() != 17) throw std::runtime_error("Oriented_extents<left_to_right, top_down, false>: breadth not adding correctly");
    }
}

template<Orientation LonOrient, Orientation LatOrient, bool YAxisBottomUp>
static void test_Oriented_rectangle_instance()
{
    static constexpr Oriented_rectangle<LonOrient, LatOrient, YAxisBottomUp> c_rect { 10, 15, 30, 20 };
    static auto rect { c_rect };
    static constexpr Oriented_rectangle<LonOrient, LatOrient, YAxisBottomUp> c_rect2;
    static constexpr Oriented_rectangle<LonOrient, LatOrient, YAxisBottomUp> c_rect3 { Rectangle{ 10, 15, 30, 20 } };

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

    test_Oriented_rectangle_instance<left_to_right, top_down , false>();
    test_Oriented_rectangle_instance<left_to_right, top_down , true >();
    test_Oriented_rectangle_instance<right_to_left, top_down , false>();
    test_Oriented_rectangle_instance<right_to_left, top_down , true >();
    test_Oriented_rectangle_instance<left_to_right, bottom_up, false>();
    test_Oriented_rectangle_instance<left_to_right, bottom_up, true >();
    test_Oriented_rectangle_instance<right_to_left, bottom_up, false>();
    test_Oriented_rectangle_instance<right_to_left, bottom_up, true >();
    test_Oriented_rectangle_instance<top_down , left_to_right, false>();
    test_Oriented_rectangle_instance<top_down , left_to_right, true >();
    test_Oriented_rectangle_instance<top_down , right_to_left, false>();
    test_Oriented_rectangle_instance<top_down , right_to_left, true >();
    test_Oriented_rectangle_instance<bottom_up, left_to_right, false>();
    test_Oriented_rectangle_instance<bottom_up, left_to_right, true >();
    test_Oriented_rectangle_instance<bottom_up, right_to_left, false>();
    test_Oriented_rectangle_instance<bottom_up, right_to_left, true >();

    {   
        std::string instance = " Oriented_rectangle<left_to_right, top_down, false>";

        constexpr static Oriented_rectangle<left_to_right, top_down, false> rect { 10, 5, 30, 18 };
        //static_assert(rect.longitude() == 10, "Oriented_rectangle<left_to_right, top_down, false>::longitude(): wrong value");
        if (rect.longitude() != 10) 
            throw std::runtime_error(instance + "::longitude(): wrong value");
        if (rect.longitude() + rect.length() != 40) 
            throw std::runtime_error(instance + ": longitude() and length() do not add up correctly");
        if (rect.latitude() != 5) 
            throw std::runtime_error(instance + "::latitude(): wrong value");
        if (rect.latitude() + rect.breadth() != 23) 
            throw std::runtime_error(instance + ": latitude() and breadth() do not add up correctly");

        Oriented_rectangle<left_to_right, top_down, false> r2 { 10, 5, 30, 18 };
        r2.change_start_of_longitudinal_segment( 15 );
        if (r2.length() != 25) 
            throw std::runtime_error(instance + ": length() wrong after change_start_of_longitudinal_segment()");
        if (r2.longitude() != 15) 
            throw std::runtime_error(instance + ": longitude() wrong after change_start_of_longitudinal_segment()");
        r2.change_end_of_longitudinal_segment( 20 );
        if (r2.length() != 5) 
            throw std::runtime_error(instance + ": length() wrong after change_end_of_longitudinal_segment()");
        if (r2.longitude() != 15) 
            throw std::runtime_error(instance + ": longitude() wrong after change_end_of_longitudinal_segment()");
        r2.change_length( 30 );
        if (r2.length() != 30) 
            throw std::runtime_error(instance + ": length() wrong after change_length()");
        if (r2.longitude() != 15) 
            throw std::runtime_error(instance + ": longitude() wrong after change_length()");

        r2.change_start_of_latitudinal_segment( 10 ); // + 5
        if (r2.breadth() != 13) 
            throw std::runtime_error(instance + ": breadth() wrong after change_start_of_latitudinal_segment()");
        if (r2.latitude() != 10) 
            throw std::runtime_error(instance + ": latitude() wrong after change_start_of_latitudinal_segment()");
        r2.change_end_of_latitudinal_segment( 30 );
        if (r2.breadth() != 20) 
            throw std::runtime_error(instance + ": breadth() wrong after change_end_of_latitudinal_segment()");
        if (r2.latitude() != 10) 
            throw std::runtime_error(instance + ": latitude() wrong after change_end_of_latitudinal_segment()");
    }

    {
        std::string instance = " Oriented_rectangle<right_to_left, top_down, false>";

        constexpr static Oriented_rectangle<right_to_left, top_down, false> rect { 10, 5, 30, 18 };
        //static_assert(rect.longitude() == 10, "Oriented_rectangle<left_to_right, top_down, false>::longitude(): wrong value");
        if (rect.longitude() != 40) 
            throw std::runtime_error(instance + "::longitude(): wrong value");
        if (rect.longitude() + rect.length() != rect.pos.x) 
            throw std::runtime_error(instance + ": longitude() and length() do not add up correctly");

        Oriented_rectangle<right_to_left, top_down, false> r2 { 10, 5, 30, 18 };
        r2.change_start_of_longitudinal_segment( 35 );
        if (r2.length() != 25) 
            throw std::runtime_error(instance + ": length() wrong after change_start_of_longitudinal_segment()");
        if (r2.longitude() != 35) 
            throw std::runtime_error(instance + ": longitude() wrong after change_start_of_longitudinal_segment()");
        r2.change_end_of_longitudinal_segment( 15 );
        if (r2.length() != 20) 
            throw std::runtime_error(instance + ": length() wrong after change_end_of_longitudinal_segment()");
        if (r2.longitude() != 35) 
            throw std::runtime_error(instance + ": longitude() wrong after change_end_of_longitudinal_segment()");
        r2.change_length( 40 );
        if (r2.length() != 40) 
            throw std::runtime_error(instance + ": length() wrong after change_length()");
        if (r2.longitude() != 35) 
            throw std::runtime_error(instance + ": longitude() wrong after change_length()");

        if (rect.latitude() != 5) 
            throw std::runtime_error(instance + "::latitude(): wrong value");
        if (rect.latitude() + rect.breadth() != 23) 
            throw std::runtime_error(instance + ": latitude() and breadth() do not add up correctly");
    }

    {
        std::string instance = " Oriented_rectangle<right_to_left, bottom_up, true>";
            // behaves the same as top_down with top-down Y axis

        constexpr static Oriented_rectangle<right_to_left, top_down, false> rect { 10, 5, 30, 18 };
        //static_assert(rect.longitude() == 10, "Oriented_rectangle<left_to_right, top_down, false>::longitude(): wrong value");
        if (rect.longitude() != 40) 
            throw std::runtime_error(instance + "::longitude(): wrong value");
        if (rect.longitude() + rect.length() != rect.pos.x) 
            throw std::runtime_error(instance + ": longitude() and length() do not add up correctly");
        if (rect.latitude() != 5) 
            throw std::runtime_error(instance + "::latitude(): wrong value");
        if (rect.latitude() + rect.breadth() != 23) 
            throw std::runtime_error(instance + ": latitude() and breadth() do not add up correctly");

        Oriented_rectangle<right_to_left, top_down, false> r2 { 10, 5, 30, 18 };
        r2.change_start_of_longitudinal_segment( 35 );
        if (r2.length() != 25) 
            throw std::runtime_error(instance + ": length() wrong after change_start_of_longitudinal_segment()");
        if (r2.longitude() != 35) 
            throw std::runtime_error(instance + ": longitude() wrong after change_start_of_longitudinal_segment()");
        if (r2.pos.x != 10) 
            throw std::runtime_error(instance + ": pos.x wrong after change_start_of_longitudinal_segment()");
        r2.change_end_of_longitudinal_segment( 5 );
        if (r2.length() != 30) 
            throw std::runtime_error(instance + ": length() wrong after change_end_of_longitudinal_segment()");
        if (r2.longitude() != 35) 
            throw std::runtime_error(instance + ": longitude() wrong after change_end_of_longitudinal_segment()");
        if (r2.pos.x != 5) 
            throw std::runtime_error(instance + ": pos.x wrong after change_end_of_longitudinal_segment()");
    }

    {   
        std::string instance = " Oriented_rectangle<left_to_right, bottom_up, false>";

        constexpr static Oriented_rectangle<left_to_right, bottom_up, false> rect { 10, 5, 30, 18 };
        //static_assert(rect.longitude() == 10, "Oriented_rectangle<left_to_right, top_down, false>::longitude(): wrong value");
        if (rect.longitude() != 10) 
            throw std::runtime_error(instance + "::longitude(): wrong value");
        if (rect.longitude() + rect.length() != 40) 
            throw std::runtime_error(instance + ": longitude() and length() do not add up correctly");
        if (rect.latitude() != 23) 
            throw std::runtime_error(instance + "::latitude(): wrong value");
        if (rect.latitude() + rect.breadth() != 5) 
            throw std::runtime_error(instance + ": latitude() and breadth() do not add up correctly");

        Oriented_rectangle<left_to_right, bottom_up, false> r2 { 10, 5, 30, 18 };

        r2.change_start_of_latitudinal_segment( 25 );
        if (r2.breadth() != 20) 
            throw std::runtime_error(instance + ": breadth() wrong after change_start_of_latitudinal_segment()");
        if (r2.latitude() != 25) 
            throw std::runtime_error(instance + ": latitude() wrong after change_start_of_latitudinal_segment()");
        r2.change_end_of_latitudinal_segment( 0 );
        if (r2.breadth() != 25) 
            throw std::runtime_error(instance + ": breadth() wrong after change_end_of_latitudinal_segment()");
        if (r2.latitude() != 25) 
            throw std::runtime_error(instance + ": latitude() wrong after change_end_of_latitudinal_segment()");
    }

    {   
        std::string instance = " Oriented_rectangle<left_to_right, top_down, true>";
            // behaves the same as top_down with bottom-up Y axis

        constexpr static Oriented_rectangle<left_to_right, top_down, true> rect { 10, 5, 30, 18 };
        //static_assert(rect.longitude() == 10, "Oriented_rectangle<left_to_right, top_down, false>::longitude(): wrong value");
        if (rect.longitude() != 10) 
            throw std::runtime_error(instance + "::longitude(): wrong value");
        if (rect.longitude() + rect.length() != 40) 
            throw std::runtime_error(instance + ": longitude() and length() do not add up correctly");
        if (rect.latitude() != 23) 
            throw std::runtime_error(instance + "::latitude(): wrong value");
        if (rect.latitude() + rect.breadth() != 5) 
            throw std::runtime_error(instance + ": latitude() and breadth() do not add up correctly");
    }

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