#pragma once

#include "./geometry.hpp"

namespace cppgui
{
    enum Axis
    {
        // Note: using "horizontal" and "vertical" rather than "x" and "y"
        horizontal,
        vertical
    };

    // Compile-time functions / traits

    template<Orientation> struct Axis_for_orientation;
    template<> struct Axis_for_orientation<left_to_right> { static constexpr Axis value = horizontal; };
    template<> struct Axis_for_orientation<right_to_left> { static constexpr Axis value = horizontal; };
    template<> struct Axis_for_orientation<top_down     > { static constexpr Axis value = vertical  ; };
    template<> struct Axis_for_orientation<bottom_up    > { static constexpr Axis value = vertical  ; };

    template<Orientation Orient, bool YAxisBottomUp> struct Axis_reversed;
    template<bool YAxisBottomUp> struct Axis_reversed<left_to_right, YAxisBottomUp> { static constexpr bool value = false; };
    template<bool YAxisBottomUp> struct Axis_reversed<right_to_left, YAxisBottomUp> { static constexpr bool value = true ; };
    template<> struct Axis_reversed<top_down, false > { static constexpr bool value = false; };
    template<> struct Axis_reversed<top_down, true  > { static constexpr bool value = true ; };
    template<> struct Axis_reversed<bottom_up, false> { static constexpr bool value = true ; };
    template<> struct Axis_reversed<bottom_up, true > { static constexpr bool value = false; };

    // Default latitudinal orientation based on longitudinal orientation

    /* TODO: this may be a temporary stop-gap solution; a final solution will likely consist of several compile-time
        functions, intended for different intentions. Research is needed in that area. */

    template<Orientation LonOrient> struct Default_latitudinal_orientation;
    template<> struct Default_latitudinal_orientation<left_to_right> { static constexpr Orientation value = top_down; };
    template<> struct Default_latitudinal_orientation<right_to_left> { static constexpr Orientation value = top_down; };
    template<> struct Default_latitudinal_orientation<top_down     > { static constexpr Orientation value = left_to_right; };
    template<> struct Default_latitudinal_orientation<bottom_up    > { static constexpr Orientation value = left_to_right; };

    // Directed_position  -------------------------------------------

    // TODO: replace with truly oriented position (forbid adding Y delta to X position) ?

    /** Synopsis: Oriented_position<> objects represent 1-dimensional positions that can either
            increase with their axis, or decrease with it.

        TODO: rename to Axis_oriented_position<> ?
     */

    struct Directed_position_base
    {
        constexpr Directed_position_base(): _p{0} {}
        constexpr /*explicit*/ Directed_position_base(Position p): _p { p } {}

        auto& operator = (Position p) { _p = p; return *this; }

        constexpr bool operator == (Position p) const { return _p == p; }

        constexpr operator Position () const { return _p; }

    protected:
        Position _p;
    };

    template<bool Reversed> struct Directed_position;

    template<> struct Directed_position<false>: Directed_position_base
    {
        using Directed_position_base::Directed_position_base;
        using Directed_position_base::operator =;

        auto operator += (Position_delta d) -> Directed_position & { _p += d; return *this; }
        constexpr auto operator + (Position_delta d) const { return Directed_position { this->_p + d }; }

        auto operator -= (Position_delta d) -> Directed_position & { _p -= d; return *this; }
        constexpr auto operator - (Position_delta d) const { return Directed_position { this->_p - d }; }
    };

    template<> struct Directed_position<true>: Directed_position_base
    {
        using Directed_position_base::Directed_position_base;
        using Directed_position_base::operator =;

        auto operator += (Position_delta d) -> Directed_position & { _p -= d; return *this; }
        constexpr auto operator + (Position_delta d) const { return Directed_position { this->_p - d }; }

        auto operator -= (Position_delta d) -> Directed_position & { _p += d; return *this; }
        //constexpr auto operator - (Position_delta d) const { return Directed_position { this->_p + d }; }
        constexpr auto operator - (Position_delta d) const { return Directed_position { this->_p + d }; }
        constexpr auto operator - (Directed_position p) const { 
            return Directed_position { p + this->_p }; // re-reversing sign!
        }
    };

    // Directed_length ----------------------------------------------

    template<bool Reversed> struct Directed_length;

    struct Directed_length_base
    {
        constexpr /*explicit*/ Directed_length_base(Length l): _l { l } {}

        auto& operator = (Length l) { _l = l; return *this; }

        constexpr bool operator == (Length l) const { return _l == l; }

        constexpr operator Position () const { return _l; }

    protected:
        Length _l;
    };

    template<> struct Directed_length<false>: Directed_length_base
    {
        using Directed_length_base::Directed_length_base;
        using Directed_length_base::operator =;

        auto operator += (Length dl) -> Directed_length & { _l += dl; return *this; }
        //constexpr auto operator + (Length dl) const { return Directed_length { _l + dl }; }

        static auto start() -> Directed_position<false> { return 0; }
    };

    template<> struct Directed_length<true>: Directed_length_base
    {
        using Directed_length_base::Directed_length_base;
        using Directed_length_base::operator =;

        auto operator += (Length dl) -> Directed_length & { _l += dl; return *this; }
        //constexpr auto operator + (Length dl) const {  return Directed_length { _l - dl }; }

        auto start() const -> Directed_position<true> { return this->_l; }
    };

    // Oriented_point -----------------------------------------------

    template<class Impl, Axis LonAxis, bool LonRev, bool LatRev> struct Oriented_point_accessor;

    template<class Impl, bool LonRev, bool LatRev> 
    struct Oriented_point_accessor<Impl, horizontal, LonRev, LatRev>
    {
        auto longitude() -> Directed_position<LonRev> & { return * reinterpret_cast<Directed_position<LonRev>*>( & p()->x ); }
        auto latitude () -> Directed_position<LatRev> & { return * reinterpret_cast<Directed_position<LatRev>*>( & p()->y ); }

    private:
        auto p() { return static_cast<Impl*>(this); }
    };

    template<class Impl, bool LonRev, bool LatRev> 
    struct Oriented_point_accessor<Impl, vertical, LonRev, LatRev>
    {
        auto longitude() -> Directed_position<LonRev> & { return * reinterpret_cast<Directed_position<LonRev>*>( & p()->y ); }
        auto latitude () -> Directed_position<LatRev> & { return * reinterpret_cast<Directed_position<LatRev>*>( & p()->x ); }

    private:
        auto p() { return static_cast<Impl*>(this); }
    };

    template<Orientation LonOrient, Orientation LatOrient, bool YAxisBottomUp> 
    struct Oriented_point_base: Point, 
        Oriented_point_accessor< Oriented_point_base<LonOrient, LatOrient, YAxisBottomUp>,
            Axis_for_orientation<LonOrient>::value, 
            Axis_reversed<LonOrient, YAxisBottomUp>::value, Axis_reversed<LatOrient, YAxisBottomUp>::value >
    {
        constexpr Oriented_point_base(): Point{} {}
        explicit constexpr Oriented_point_base(const Point &from): Point{ from } {}
        explicit constexpr Oriented_point_base(Point &&from): Point{ from } {}
    };

    template<Orientation LonOrient, Orientation LatOrient, bool YAxisBottomUp> 
    struct Oriented_point : Oriented_point_base<LonOrient, LatOrient, YAxisBottomUp>
    {       
        using Oriented_point_base::Oriented_point_base;
    };

    // Oriented_extents<> -------------------------------------------

    template<class Impl, Axis LonAxis, bool LonRev, bool LatRev> struct Oriented_extents_accessor;

    template<class Impl, bool LonRev, bool LatRev> 
    struct Oriented_extents_accessor<Impl, horizontal, LonRev, LatRev>
    {
        auto length () -> Directed_length<LonRev> & { return * reinterpret_cast<Directed_length<LonRev>*>( & p()->w ); }
        auto breadth() -> Directed_length<LatRev> & { return * reinterpret_cast<Directed_length<LatRev>*>( & p()->h ); }

    private:
        auto p() { return static_cast<Impl*>(this); }
    };

    template<class Impl, bool LonRev, bool LatRev> 
    struct Oriented_extents_accessor<Impl, vertical, LonRev, LatRev>
    {
        auto length () -> Directed_length<LonRev> & { return * reinterpret_cast<Directed_length<LonRev>*>( & p()->h ); }
        auto breadth() -> Directed_length<LatRev> & { return * reinterpret_cast<Directed_length<LatRev>*>( & p()->w ); }

    private:
        auto p() { return static_cast<Impl*>(this); }
    };

    template<Orientation LonOrient, Orientation LatOrient, bool YAxisBottomUp> 
    struct Oriented_extents_base: Extents, 
        Oriented_extents_accessor< Oriented_extents_base<LonOrient, LatOrient, YAxisBottomUp>,
        Axis_for_orientation<LonOrient>::value, 
        Axis_reversed<LonOrient, YAxisBottomUp>::value, Axis_reversed<LatOrient, YAxisBottomUp>::value >
    {
        constexpr Oriented_extents_base(): Extents{} {}
        explicit constexpr Oriented_extents_base(const Extents &from): Extents{ from } {}
        explicit constexpr Oriented_extents_base(Extents &&from): Extents{ from } {}
    };

    template<Orientation LonOrient, Orientation LatOrient, bool YAxisBottomUp> 
    struct Oriented_extents : Oriented_extents_base<LonOrient, LatOrient, YAxisBottomUp>
    {       
        using Oriented_extents_base::Oriented_extents_base;
    };

    // Oriented_rectangle<> -----------------------------------------

    // Rectangle_axis_data_accessor: gives access to either horizontal or vertical data (position + length)

    template<class Impl, bool AxisVert> struct Rectangle_axis_data_accessor;

    template<class Impl> struct Rectangle_axis_data_accessor<Impl, false>
    {
        constexpr auto& position_on_axis() const { return static_cast<const Impl*>(this)->pos.x; }
                  auto& position_on_axis()       { return static_cast<      Impl*>(this)->pos.x; }
        constexpr auto& length_on_axis  () const { return static_cast<const Impl*>(this)->ext.w; }
                  auto& length_on_axis  ()       { return static_cast<      Impl*>(this)->ext.w; }
    };

    template<class Impl> struct Rectangle_axis_data_accessor<Impl, true>
    {
        /*
        using Position_t = Directed_position<false>;
        using Length_t   = Directed_length  <false>;

        constexpr auto& position_on_axis() const { return *reinterpret_cast<const Position_t*>( & static_cast<const Impl*>(this)->pos.y ); }
                  auto& position_on_axis()       { return *reinterpret_cast<      Position_t*>( & static_cast<      Impl*>(this)->pos.y ); }
        constexpr auto& length_on_axis  () const { return *reinterpret_cast<const Length_t  *>( & static_cast<const Impl*>(this)->ext.h ); }
                  auto& length_on_axis  ()       { return *reinterpret_cast<      Length_t  *>( & static_cast<      Impl*>(this)->ext.h ); }
        */

        constexpr auto& position_on_axis() const { return static_cast<const Impl*>(this)->pos.y; }
                  auto& position_on_axis()       { return static_cast<      Impl*>(this)->pos.y; }
        constexpr auto& length_on_axis  () const { return static_cast<const Impl*>(this)->ext.h; }
                  auto& length_on_axis  ()       { return static_cast<      Impl*>(this)->ext.h; }
    };

    // Oriented_rectangle_longitude_accessor: handles difference between non-reversed / reversed longitude
    
    template<class Impl, bool LonVert, bool LonRev> struct Oriented_rectangle_longitude_accessor;

    template<class Impl, bool LonVert>
    struct Oriented_rectangle_longitude_accessor<Impl, LonVert, false>: Rectangle_axis_data_accessor<Impl, LonVert>
    {
        //----
        // This is the specialization for a NON-REVERSED longitudinal axis.
        //---

        using Position_t = Directed_position<false>;
        using Length_t   = Directed_length  <false>;

        constexpr auto longitude() const { return static_cast<Position_t>( this->position_on_axis() ); }
        constexpr auto length   () const { return static_cast<Length_t  >( this->length_on_axis  () ); }

        void  set_longitudinal_segment(Position p, Length l)
        {
            this->position_on_axis() = p; 
            this->length_on_axis() = l;
        }
        void set_longitude(Position p)
        {
            this->position_on_axis() = p;
        }
        void  change_start_of_longitudinal_segment(Position p)
        {
            auto d = p - this->position_on_axis(); 
            this->position_on_axis() = p; 
            this->length_on_axis() -= d;
        }
        void  change_end_of_longitudinal_segment  (Position p)
        {
            assert(p > this->position_on_axis()); 
            this->length_on_axis() = p - this->position_on_axis();
        }
        void change_length(Length l)
        {
            this->length_on_axis() = l;
        }
    };

    template<class Impl, bool LonVert> 
    struct Oriented_rectangle_longitude_accessor<Impl, LonVert, true>: Rectangle_axis_data_accessor<Impl, LonVert>
    {
        //----
        // This is the specialization for a REVERSED longitudinal axis.
        //---

        using Position_t = Directed_position<true>;
        using Length_t   = Directed_length  <true>;

        constexpr auto longitude() const { return static_cast<Position_t>( this->position_on_axis() + this->length_on_axis() ); }
        constexpr auto length   () const { return static_cast<Length_t  >( this->length_on_axis  () ); }

        void  set_longitudinal_segment(Position p, Length l)
        {
            this->position_on_axis() = p - l; // p + l; 
            this->length_on_axis() = l;
        }
        void set_longitude(Position p)
        {
            this->position_on_axis() = p - this->length_on_axis();
        }
        void  change_start_of_longitudinal_segment(Position p)
        {
            this->length_on_axis() = p - this->position_on_axis(); 
            this->position_on_axis() = p - this->length_on_axis();
        }
        void  change_end_of_longitudinal_segment  (Position p)
        {
            this->length_on_axis() = this->position_on_axis() + this->length_on_axis() - p;
            this->position_on_axis() = p;
        }
        void change_length(Length l)
        {
            this->position_on_axis() -= l - this->length_on_axis();
            this->length_on_axis() = l;
        }
    };

    template<class Impl, bool LonVert, bool LatRev> struct Oriented_rectangle_latitude_accessor;

    template<class Impl, bool LonVert> 
    struct Oriented_rectangle_latitude_accessor<Impl, LonVert, false>: Rectangle_axis_data_accessor<Impl, !LonVert /* other axis */>
    {
        using Position_t = Directed_position<false>;
        using Length_t   = Directed_length  <false>;

        constexpr auto latitude() const { return static_cast<Position_t>( this->position_on_axis() ); }
        constexpr auto breadth () const { return static_cast<Length_t  >( this->length_on_axis  () ); }

        void  set_latitudinal_segment(Position p, Length l)
        {
            this->position_on_axis() = p; 
            this->length_on_axis() = l;
        }
        void  change_start_of_latitudinal_segment(Position p)
        {
            auto d = p - this->position_on_axis(); 
            this->position_on_axis() = p; 
            this->length_on_axis() -= d;
        }
        void  change_end_of_latitudinal_segment  (Position p)
        {
            assert(p > this->position_on_axis()); 
            this->length_on_axis() = p - this->position_on_axis();
        }
    };

    template<class Impl, bool LonVert> 
    struct Oriented_rectangle_latitude_accessor<Impl, LonVert, true>: Rectangle_axis_data_accessor<Impl, !LonVert /* other axis */>
    {
        using Position_t = Directed_position<true>;
        using Length_t   = Directed_length  <true>;

        constexpr auto latitude() const { return static_cast<Position_t>( this->position_on_axis() + this->length_on_axis() ); }
        constexpr auto breadth () const { return static_cast<Length_t  >( this->length_on_axis  () ); }

        void  set_latitudinal_segment(Position p, Length l)
        {
            this->position_on_axis() = p + l; 
            this->length_on_axis() = l;
        }
        void  change_start_of_latitudinal_segment(Position p)
        {
            this->length_on_axis() = p - this->position_on_axis(); 
            this->position_on_axis() = p - this->length_on_axis();
        }
        void  change_end_of_latitudinal_segment  (Position p)
        {
            this->length_on_axis() = this->position_on_axis() + this->length_on_axis() - p;
            this->position_on_axis() = p;
        }
    };

    // Oriented_rectangle_base<>: handles axes and their directions

    template<bool LonVert, bool LonRev, bool LatRev> 
    struct Oriented_rectangle_base: Rectangle,
        Oriented_rectangle_longitude_accessor< Oriented_rectangle_base<LonVert, LonRev, LatRev>, LonVert, LonRev>,
        Oriented_rectangle_latitude_accessor < Oriented_rectangle_base<LonVert, LonRev, LatRev>, LonVert, LatRev>
    {
        using Rectangle::Rectangle;

        constexpr Oriented_rectangle_base(): Rectangle{} {}

        constexpr explicit Oriented_rectangle_base(const Rectangle &from): Rectangle{ from } {}

        //constexpr operator const Rectangle & () const { return static_cast<const Rectangle &>(*this); }
        //operator Rectangle & () { return static_cast<Rectangle &>(*this); }
    };

    // Final product: Oriented_rectangle<>, parameterized by axes orientations and Y bottom up flag

    template<Orientation LonOrient, Orientation LatOrient, bool YAxisBottomUp>
    struct Oriented_rectangle: Oriented_rectangle_base< 
        LonOrient == top_down || LonOrient == bottom_up, // TODO: use a constexpr function
        Axis_reversed<LonOrient, YAxisBottomUp>::value, 
        Axis_reversed<LatOrient, YAxisBottomUp>::value
    >
    {
        using Point_t   = Oriented_point  <LonOrient, LatOrient, YAxisBottomUp>;
        using Extents_t = Oriented_extents<LonOrient, LatOrient, YAxisBottomUp>;

        using Oriented_rectangle_base::Oriented_rectangle_base;

        constexpr auto& position() const { return * reinterpret_cast<const Point_t  *>( &this->pos ); }
                  auto& position()       { return * reinterpret_cast<      Point_t  *>( &this->pos ); }

        constexpr auto& extents () const { return * reinterpret_cast<const Extents_t*>( &this->ext ); }
                  auto& extents ()       { return * reinterpret_cast<      Extents_t*>( &this->ext ); }
    };

} // ns cppgui
