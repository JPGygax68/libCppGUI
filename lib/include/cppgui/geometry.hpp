#pragma once

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

#include <array>
#include <cassert>

#include "./utils.hpp"

namespace cppgui {

    // 1D ----------------------------

    enum Horizontal_direction
    {
        right,
        left
    };

    enum Vertical_direction
    {
        up,
        down
    };

    using Position = int;
    using Position_delta = Position;
    using Length = int;
    using Width = Length;

    // 2D ----------------------------

    enum Axis
    {
        // Note: using "horizontal" and "vertical" rather than "x" and "y"
        horizontal,
        vertical
    };

    enum Orientation
    {
        none,
        left_to_right,
        bottom_up,
        right_to_left,
        top_down
    };

    template<Orientation Or> struct Axis_for_orientation;
    template<> struct Axis_for_orientation<left_to_right> { static constexpr Axis value = horizontal; };
    template<> struct Axis_for_orientation<right_to_left> { static constexpr Axis value = horizontal; };
    template<> struct Axis_for_orientation<top_down     > { static constexpr Axis value = vertical  ; };
    template<> struct Axis_for_orientation<bottom_up    > { static constexpr Axis value = vertical  ; };

    template<Orientation Or> struct Axis_reversed;
    template<> struct Axis_reversed<left_to_right> { static constexpr bool value = false; };
    template<> struct Axis_reversed<right_to_left> { static constexpr bool value = true ; };
    template<> struct Axis_reversed<top_down     > { static constexpr bool value = false; }; // top-down arbitrarily defined as "standard"
    template<> struct Axis_reversed<bottom_up    > { static constexpr bool value = true ; };

    struct _oriented_position_base
    {
        explicit _oriented_position_base(Position p_): p{ p_ } {}

        operator Position () const { return p; }

    protected:
        Position p;
    };

    template<bool AxisReversed> struct Oriented_position;

    template<> struct Oriented_position<false>: _oriented_position_base {

        explicit Oriented_position(Position p_): _oriented_position_base(p_) {}

        auto operator + (Position_delta d) { return p + d; }
    };

    template<> struct Oriented_position<true>: _oriented_position_base {

        explicit Oriented_position(Position p_): _oriented_position_base(p_) {}

        auto operator + (Position_delta d) { return p - d; }
    };

    struct Extents;

    struct Point {

        Position x, y;

        auto operator + (const Point &delta) const { return Point{ x + delta.x, y + delta.y }; }
        auto operator - (const Point &delta) const { return Point{ x - delta.x, y - delta.y }; }
        auto& operator += (const Point &delta) { x += delta.x, y += delta.y; return *this; }

        auto operator + (const Extents & ext) const -> Point;
        auto operator - (const Extents & ext) const -> Point;
    };

    using Vector = Point;

    using Extents_delta = Point; // TODO: define specialized class

    using Text_origin = Point;

    using Padding = std::array<Width, 4>;
    using Margins = Padding;

    struct Extents {

        Length w, h;

        Extents(): w { 0 }, h { 0 } {}
        Extents(Length w_, Length h_): w { w_ }, h { h_ } {}
        Extents(const Extents &from): w { from.w }, h { from.h } {}
        Extents(Extents &&) = default;

        auto operator = (const Extents &) -> Extents & = default;
        auto operator = (Extents &&) -> Extents & = default;

        auto right_edge () const { return static_cast<Position>(w); }
        auto bottom_edge() const { return static_cast<Position>(h); }

        bool contains(const Point &pos) const { 

            return pos.x >= 0 && pos.y >= 0 
                && static_cast<Length>(pos.x) < w && static_cast<Length>(pos.y) < h; 
        }

        bool can_contain(const Extents &rect) const
        {
            return rect.w < w && rect.h < h;
        }

        auto operator + (const Extents_delta &delta) const -> Extents {

            assert(static_cast<Position>(w) + delta.x >= 0);
            assert(static_cast<Position>(h) + delta.y >= 0);

            return { 
                static_cast<Length>(static_cast<Position>(w) + delta.x), 
                static_cast<Length>(static_cast<Position>(h) + delta.y) 
            };
        }

        auto operator - (const Extents_delta &delta) const -> Extents {

            assert(static_cast<Position>(w) - delta.x >= 0);
            assert(static_cast<Position>(h) - delta.y >= 0);

            return { 
                static_cast<Length>(static_cast<Position>(w) - delta.x), 
                static_cast<Length>(static_cast<Position>(h) - delta.y) 
            };
        }

        auto operator += (const Padding &padding)
        {
            w += padding[3] + padding[1], h += padding[0] + padding[1];
            return *this;
        }

        auto right () { return static_cast<Position>(w); }
        auto bottom() { return static_cast<Position>(h); }

        static auto between_points(const Point &from, const Point &to) -> Extents {

            return { static_cast<Length>(to.x - from.x), static_cast<Length>(to.y - from.y) };
        }
    };

    inline auto Point::operator+(const Extents & ext) const -> Point
    {
        return {x + ext.w, y + ext.h};
    }

    inline auto Point::operator-(const Extents & ext) const -> Point
    {
        return {x - ext.w, y - ext.h};
    }

    struct Rectangle {

        Point pos;
        Extents  ext;

        Rectangle(): pos {}, ext {} {}
        Rectangle(const Point &pos_, const Extents &ext_): pos {pos_}, ext { ext_ } {}
        Rectangle(const Extents &ext_): pos {}, ext {ext_} {}
        Rectangle(const Rectangle &) = default;
        Rectangle(Rectangle &&) = default;
        Rectangle(Position x, Position y, Length w, Length h): pos { x, y}, ext { w, h } {}

        auto operator = (const Rectangle &) -> Rectangle & = default;
        auto operator = (Rectangle &&) -> Rectangle & = default;

        bool contains(const Point &p) const { return ext.contains(p - pos); }

        /** Access conveniences.
        */
        ///@{

        auto width () const { return ext.w; }
        auto height() const { return ext.h; }
        auto position() -> Point   & { return pos; }
        auto extents () -> Extents & { return ext; }

        ///@}

        /** Computation conveniences.
        */
        ///@{

        auto top_left() const 
        { 
            return pos; 
        }
        auto top_right() const -> Point
        { 
            return { pos.x + static_cast<Position>(ext.w), pos.y }; 
        }
        auto bottom_right() const -> Point 
        { 
            return { pos.x + static_cast<Position>(ext.w), pos.y + static_cast<Position>(ext.h) }; 
        }
        auto bottom_left() const -> Point 
        { 
            return { pos.x, pos.y + static_cast<Position>(ext.h) }; 
        }
        auto left() const 
        { 
            return pos.x; 
        }
        auto top() const 
        { 
            return pos.y; 
        }
        auto right() const 
        { 
            return pos.x + static_cast<Position>(ext.w); 
        }
        auto bottom() const 
        { 
            return pos.y + static_cast<Position>(ext.h); 
        }

        // Manipulation

        void set_left  (Position x) { assert(x < pos.x + ext.w); pos.x = x; }
        void set_right (Position x) { assert(x >= pos.x); ext.w = x - pos.x; }
        void set_top   (Position y) { assert(y < pos.y + ext.h); pos.y = y; }
        void set_bottom(Position y) { assert(y >= pos.y); ext.h = y - pos.y; }

        ///@}

        /** Adding a Point to a rectangle will move its position without changing its extents.
        */
        ///@{

        auto operator + (const Point &offs) const -> Rectangle { return { pos.x + offs.x, pos.y + offs.y, ext.w, ext.h }; }
        auto operator - (const Point &offs) const -> Rectangle { return { pos.x - offs.x, pos.y - offs.y, ext.w, ext.h }; }

        ///@}

        /* 
        auto shrink(Length l) const -> Rectangle { return shrink({ l, l }); }

        auto shrink(const Extents &delta) const -> Rectangle { 

        return { 
        pos.x + static_cast<Position>(delta.w), pos.y + static_cast<Position>(delta.h), 
        ext.w - 2 * delta.w, ext.h - 2 * delta.h 
        }; 
        }
        */

        /** shrink() methods: make rectangle smaller
        */
        ///@{

        void shrink(Length l) 
        { 
            shrink( Extents{ l, l } ); 
        }
        void shrink(const Extents &delta)
        { 
            pos.x += static_cast<Position_delta>(delta.w), pos.y += static_cast<Position_delta>(delta.h);
            ext.w -= 2 * delta.w, ext.h -= 2 * delta.h;
        }
        void shrink(const Padding &padding)
        {
            pos.x += static_cast<Position>(padding[3]), pos.y += static_cast<Position>(padding[0]);
            ext.w -= padding[3] + padding[1], ext.h -= padding[0] + padding[2];
        }

        ///@}

        /** operator - variants: equivalent to shrink(..) methods except that they return 
        a new rectangle instance, leaving the original unchanged.
        */
        ///@{

        auto operator - (Width w) const -> Rectangle
        {
            return { 
                pos.x + static_cast<Position_delta>(w), pos.y + static_cast<Position_delta>(w), 
                ext.w - 2 * w, ext.h - 2 * w
            }; 
        }
        auto operator - (const Extents &delta) const -> Rectangle 
        { 
            return { 
                pos.x + static_cast<Position>(delta.w), pos.y + static_cast<Position>(delta.h), 
                ext.w - 2 * delta.w, ext.h - 2 * delta.h 
            }; 
        }
        auto operator - (const Padding &padding) const -> Rectangle 
        { 
            return { 
                pos.x + static_cast<Position>(padding[3]), pos.y + static_cast<Position>(padding[0]), 
                ext.w - padding[3] - padding[1], ext.h - padding[0] - padding[2]
            }; 
        }

        ///@}

        /** grow(): make the rectangle bigger
        */
        //@{

        void grow(Width w) 
        { 
            grow( Extents{ w, w } ); 
        }
        void grow(const Extents &delta)
        { 
            pos.x -= static_cast<Position>(delta.w), pos.y -= static_cast<Position>(delta.h);
            ext.w += 2 * delta.w, ext.h += 2 * delta.h;
        }
        void grow(const Margins &margins)
        {
            pos.x -= static_cast<Position>(margins[3]), pos.y -= static_cast<Position>(margins[0]);
            ext.w += margins[3] + margins[1], ext.h += margins[0] + margins[2];
        }

        ///@}

        /** operator + variants: equivalent to grow(..) methods except that they return 
        a new rectangle instance, leaving the original unchanged.
        */
        ///@{

        auto operator + (Length l) const -> Rectangle 
        { 
            return *this + Extents{ l, l }; 
        }
        auto operator + (const Extents &delta) const -> Rectangle 
        { 
            return { 
                pos.x - static_cast<Position>(delta.w), pos.y - static_cast<Position>(delta.h), 
                ext.w + 2 * delta.w, ext.h + 2 * delta.h 
            }; 
        }
        auto operator + (const Padding &padding) const -> Rectangle 
        { 
            return { 
                pos.x - static_cast<Position>(padding[3]), pos.y - static_cast<Position>(padding[0]), 
                ext.w + padding[3] + padding[1], ext.h + padding[0] + padding[1]
            }; 
        }

        ///@}

    };

    #ifdef OBSOLETE_CODE

    template<class Impl>
    struct Geometry_accessor_base
    {
        static constexpr auto add_forward(const Point &point, Position_delta delta)
        {
            return point + Impl::forward_vector(delta);
        }

        static constexpr auto add_forward(const Rectangle &rect, Position_delta delta)
        {
            return rect + Impl::forward_vector(delta);
        }

        static constexpr auto add_sideways(const Point &point, Position_delta delta)
        {
            return point + Impl::sideways_vector(delta);
        }

        static constexpr auto add_sideways(const Rectangle &rect, Position_delta delta)
        {
            return rect + Impl::sideways_vector(delta);
        }

        static constexpr void set_oriented_position(Point &ext, Position forward, Position sideways)
        {
            Impl::forward_position (ext) = forward;
            Impl::sideways_position(ext) = sideways;
        }

        static constexpr void set_oriented_extents(Extents &ext, Length l, Length w)
        {
            Impl::forward_length(ext) = l;
            Impl::sideways_width(ext) = w;
        }
    };

    struct Horizontal_geometry_accessor: Geometry_accessor_base<Horizontal_geometry_accessor>
    {
        static constexpr auto forward_vector (Position_delta delta) -> Vector { return { delta, 0 }; }
        static constexpr auto sideways_vector(Position_delta delta) -> Vector { return { 0, delta }; }

        template<typename T> 
        static constexpr auto forward_position (T &point) -> enable_member_ref_for_class_t<T, Point, Position> & { return point.x; }
        template<typename T> 
        static constexpr auto sideways_position(T &point) -> enable_member_ref_for_class_t<T, Point, Position> & { return point.y; }

        template<typename T> 
        static constexpr auto forward_length(T &ext) -> enable_member_ref_for_class_t<T, Extents, Length> & { return ext.w; }
        template<typename T> 
        static constexpr auto sideways_width(T &ext) -> enable_member_ref_for_class_t<T, Extents, Length> & { return ext.h; }
    };

    struct Vertical_geometry_accessor: Geometry_accessor_base<Vertical_geometry_accessor>
    {
        static constexpr auto forward_vector (Position_delta delta) -> Vector { return { 0, delta }; }
        static constexpr auto sideways_vector(Position_delta delta) -> Vector { return { delta, 0 }; }

        template<typename T> 
        constexpr static auto forward_position (T &point) -> enable_member_ref_for_class_t<T, Point, Position> & { return point.y; }               
        template<typename T> 
        constexpr static auto sideways_position(T &point) -> enable_member_ref_for_class_t<T, Point, Position> & { return point.x; }               
        template<typename T> 
        constexpr static auto forward_length(T &ext) -> enable_member_ref_for_class_t<T, Extents, Length> & { return ext.h; }
        template<typename T> 
        constexpr static auto sideways_width(T &ext) -> enable_member_ref_for_class_t<T, Extents, Length> & { return ext.w; }
    };

    #endif

    // Oriented positions

    template <bool Reverse> struct Oriented_position;

    template <> 
    struct Oriented_position<false>
    {
        explicit Oriented_position(Position p): _p{ p } {}

        operator Position () const { return _p; }

        auto& operator + (Position d) { _p += d; return *this; }
        auto& operator - (Position d) { _p -= d; return *this; }

        Position _p;
    };

    template <> 
    struct Oriented_position<true>
    {
        explicit Oriented_position(Position p): _p{ p } {}

        operator Position () const { return _p; }

        auto& operator + (Position d) { _p -= d; return *this; }
        auto& operator - (Position d) { _p += d; return *this; }

        Position _p;
    };


    using Latitude = Position;

    // Oriented point

    // TODO: degrade orientation to horizontal/vertical

    template<Orientation Orientation> struct Oriented_point {};

    template<>
    struct Oriented_point<left_to_right>: Point
    {
        Oriented_point() = default;
        explicit Oriented_point(Position p1, Position p2 = 0): Point{ p1, p2 } {}
        explicit Oriented_point(const Point &pt): Point{ pt } {}

        auto& longitude()       { return this->x; }
        auto  longitude() const { return this->x; }
        auto& latitude ()       { return this->y; }
        auto  latitude () const { return this->y; }

        auto longitude_diff(Position p) { return longitude() - p; }
    };

    template<>
    struct Oriented_point<bottom_up>: Point
    {
        Oriented_point() = default;
        explicit Oriented_point(Position p1, Position p2 = 0): Point{ p2, p1 } {}
        explicit Oriented_point(const Point &from): Point{ from } {}
        explicit Oriented_point(Point &&from): Point{ from } {}

        auto& longitude()       { return this->y; }
        auto  longitude() const { return this->y; }
        auto& latitude ()       { return this->x; }
        auto  latitude () const { return this->x; }

        auto longitude_diff(Position p) { return p - longitude(); }
    };

    // Oriented extents

    template<Orientation Orientation> struct Oriented_extents {};

    template<>
    struct Oriented_extents<left_to_right>: Extents
    {
        Oriented_extents() {}
        Oriented_extents(Length w, Length h): Extents{ w, h } {}
        explicit Oriented_extents(const Oriented_extents &from) = default;
        explicit Oriented_extents(const Extents &from): Extents{ from.w, from.h } { }

        auto& length()       { return this->w; }
        auto  length() const { return this->w; }
        auto& width ()       { return this->h; }
        auto  width () const { return this->h; }

        void set(Length l, Width w_) { this->w = l, this->h = w_; }
    };

    template<>
    struct Oriented_extents<bottom_up>: Extents
    {
        Oriented_extents() {}
        Oriented_extents(Length w, Length h): Extents{ h, w } {}
        explicit Oriented_extents(const Oriented_extents &from) = default;
        explicit Oriented_extents(const Extents &from): Extents{ from.w, from.h } { }

        auto& length()       { return this->h; }
        auto  length() const { return this->h; }
        auto& width ()       { return this->w; }
        auto  width () const { return this->w; }

        void set(Length l, Width w_) { this->h = l, this->w = w_; }
    };

    // Oriented rectangles

    #ifdef NOT_DEFINED

    template<class Impl, Orientation Orientation>
    struct Oriented_rectangle_base: Rectangle
    {
        using Rectangle::Rectangle;

        explicit Oriented_rectangle_base(const Oriented_extents<Orientation> &ext = {}) { this->extents() = ext; }

        Oriented_rectangle_base(const Oriented_point<Orientation> &pos, const Oriented_extents<Orientation> &ext)
        {
            this->position() = pos;
            this->extents () = ext;
        }

        explicit Oriented_rectangle_base(const Rectangle &from): Rectangle{ from } {}

        void set(Position p1, Position p2, Length l, Width w)
        {
            Impl::set_first_axis (p1, l);
            Impl::set_second_axis(p2, w);
        }

        auto& point  () { return static_cast<Oriented_point  <Orientation> &>(this->pos); }
        auto& extents() { return static_cast<Oriented_extents<Orientation> &>(this->ext); }

    protected:

        auto p()       { return static_cast<      Impl*>(this); }
        auto p() const { return static_cast<const Impl*>(this); }
    };

    template<Orientation Orientation> struct Oriented_rectangle {};

    // Origin is top left corner, positive is to the right and down

    template<>
    struct Oriented_rectangle<left_to_right>: Oriented_rectangle_base<Oriented_rectangle<left_to_right>, left_to_right>
    {
        //operator Rectangle & () { return * static_cast<Oriented_rectangle_base*>(this); }
        //operator const Rectangle & () const { return * static_cast<const Rectangle *>(this); }

        using Oriented_rectangle_base::Oriented_rectangle_base;

        auto longitude() const { return this->pos.x; }
        auto length   () const { return this->ext.w; }
        auto latitude () const { return this->pos.y; }
        auto width    () const { return this->ext.h; }

        void set_lon_seg(Position p, Length l) { this->pos.x = p, this->ext.w = l; }
        void set_lat_seg(Position p, Width  w) { this->pos.y = p, this->ext.h = w; }

        // TODO: get rid of the following two
        auto first_axis_end    () const -> Position { return this->pos.x + this->ext.w; }
        auto second_axis_end   () const -> Position { return this->pos.y + this->ext.h; }

        auto delta_from_longitude_origin (Position_delta d) const -> Position_delta { return d; }

        auto longitude_to_vector(Position_delta p) const -> Vector { return { p, 0 }; }
        auto latitude_to_vector (Position_delta p) const -> Vector { return { 0, p }; }

        void move_longitude_start_by(Position_delta d) { this->pos.x += d; this->ext.w -= d; }
        void set_length(Length l) { this->ext.w = l; }

        auto define_relative_rectangle(Longitude lon, Length l, Latitude lat, Width w)
        {
            // TODO: generalize (i.e. delegate to Oriented_extents::define_relative_point() etc.), move to base class
            Oriented_rectangle res;
            res.pos.x = lon;
            res.pos.y = lat;
            res.ext.w = l;
            res.ext.h = w;
            return res;
        }
    };

    // Origin is the bottom left corner, positive is up and to the right

    template<>
    struct Oriented_rectangle<bottom_up>: Oriented_rectangle_base<Oriented_rectangle<bottom_up>, bottom_up>
    {
        using Oriented_rectangle_base::Oriented_rectangle_base;

        auto& operator = (const Rectangle &r) { this->pos = r.pos; this->ext = r.ext; return *this; }
        auto& operator = (Rectangle &&r) { this->pos = std::move(r.pos); this->ext = std::move(r.ext); return *this; }
        auto  operator = (const Oriented_rectangle &) -> Oriented_rectangle & = default;
        auto& operator = (const Oriented_extents<bottom_up> &ext_) { this->ext = ext_; return *this; }

        auto define_relative_rectangle(Longitude lon, Length l, Latitude lat, Width w)
        {
            // TODO: generalize (i.e. delegate to Oriented_extents::define_relative_point() etc.), move to base class
            Oriented_rectangle res;
            res.pos.x = lat;
            res.pos.y = this->ext.h - lon - l;
            res.ext.w = w;
            res.ext.h = l;
            return res;
        }

        void set_lon_seg(Position p, Length l) { this->pos.y = p - l, this->ext.h = l; }
        void set_lat_seg(Position p, Width  w) { this->pos.x = p, this->ext.w = w; }

        auto longitude() const -> Position { return this->pos.y + this->ext.h; }
        auto length   () const -> Length   { return this->ext.h; }
        auto latitude () const -> Position { return this->pos.x; }
        auto width    () const -> Length   { return this->ext.w; }

        // TODO: remove the following two
        auto first_axis_end    () const -> Position { return this->pos.y; }
        auto second_axis_end   () const -> Position { return this->pos.x + this->ext.w; }

        auto delta_from_longitude_origin (Position_delta d) const -> Position_delta { return this->ext.h - d; }
        //auto delta_from_first_axis_end   (Position_delta d) const -> Position_delta { return d; }
        //auto vector_from_first_axis_end  (Position_delta d) const -> Vector { return { 0, delta_from_first_axis_end  (d) }; }

        auto longitude_to_vector(Position_delta p) const -> Vector { return { 0, delta_from_longitude_origin(p) }; }
        auto latitude_to_vector (Position_delta p) const -> Vector { return { p, 0 }; }

        void move_longitude_start_by(Position_delta d) { this->ext.h -= d; }
        void set_length(Length   l) { auto delta = l - length(); this->pos.y -= delta; this->ext.h += delta; }

        void set_first_axis_center(Position p) { this->pos.y = p - this->ext.h / 2; }
    };

    #endif

} // ns cppgui

namespace std {

    inline auto max(const cppgui::Extents &a, const cppgui::Extents &b) -> cppgui::Extents
    {
        return { max(a.w, b.w), max(a.h, b.h) };
    }

} // ns std

