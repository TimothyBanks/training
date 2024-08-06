
#include <array>
#include <deque>
#include <initializer_list>
#include <iostream>
#include <training/asml/polygon.hpp>
#include <type_traits>
#include <vector>

// To compile with g++:
//     g++ -o Polygon.exe Polygon.cpp
// The functions to implement are:
// PolygonT.h
//  SegmentIntersection,
//  PolygonT::InPolygonTest, and
//  PolygonT::ClipSegments
//
// SegmentIntersection:
//  calculate the intersection(s) of two line segments
// PolygonT::InPolygonTest:
//  return the status of a point with respect to the polygon. It can be inside,
//  outside or on the polygon edge.
// PolygonT::ClipSegments
//  use the polygon to clip a path, or a consecutive set of points which define
//  a set of line segments by each pair of adjacent points of the path
//
// Polygon.cpp
//  Enhance the code in the testing classes that specialize PolygonT as needed

namespace detail {
template <typename T>
struct is_array : std::false_type {};

template <typename T>
struct is_array<std::vector<T>> : std::true_type {};

template <typename T, size_t N>
struct is_array<std::array<T, N>> : std::true_type {};

template <typename... T>
static constexpr auto is_array_v = is_array<T...>::value;

template <typename T>
struct is_pair : std::false_type {};

template <typename T>
struct is_pair<std::pair<T, T>> : std::true_type {};

template <typename T>
static constexpr auto is_pair_v = is_pair<T>::value;

template <typename T>
struct is_boost_point : std::false_type {};

template <typename T>
struct is_boost_point<boost::geometry::model::d2::point_xy<T>> : std::true_type {};

template <typename T>
static const auto is_boost_point_v = is_boost_point<T>::value;

template <typename T, typename Enable = void>
struct point_trait;

template <typename T>
struct point_trait<T, typename std::enable_if_t<is_array_v<T>>> {
  using container_type = T;
  using coord_type = typename container_type::value_type;

  static container_type make(coord_type x, coord_type y) {
    return container_type{x, y};
  }

  static container_type make(const std::initializer_list<coord_type>& l) {
    auto c = container_type{};
    auto it = std::cbegin(l);
    c[0] = *it;
    ++it;
    c[1] = *it;
    return c;
  }

  static coord_type& x(container_type& data) { return data[0]; }

  static coord_type& y(container_type& data) { return data[1]; }
};


// TODO:  Something to consider is to store floating points as integral types with a 
//        fixed precision and perform the operations on intergral types.
template <typename T>
struct point_trait<T,
                   typename std::enable_if_t<std::is_integral_v<T> ||
                                             std::is_floating_point_v<T>>> {
  using coord_type = T;
  struct point {
    point() = default;
    point(const point&) = default;
    point(point&&) = default;
    point(coord_type x_, coord_type y_) : x{x_}, y{y_} {}
    point(const std::initializer_list<coord_type>& l)
        : x{[&]() {
            if (l.size() > 1) {
              return *std::cbegin(l);
            }
            return coord_type{};
          }()},
          y{[&]() {
            if (l.size() > 2) {
              auto it = std::cbegin(l);
              ++it;
              return *it;
            }
            return coord_type{};
          }()} {}

    point& operator=(const point&) = default;
    point& operator=(point&&) = default;

    coord_type x{};
    coord_type y{};
  };
  using container_type = point;

  static container_type make(coord_type x, coord_type y) { return point{x, y}; }

  static container_type make(const std::initializer_list<coord_type>& l) {
    return point{l};
  }

  static coord_type& x(container_type& data) { return data.x; }

  static coord_type& y(container_type& data) { return data.y; }
};

template <typename T>
struct point_trait<T, typename std::enable_if_t<is_pair_v<T>>> {
  static_assert(std::is_same_v<typename T::first_type, typename T::second_type>,
                "Pair must contain the same type for first and second");
  using coord_type = typename T::first_type;
  using container_type = T;

  static container_type make(coord_type x, coord_type y) {
    return std::make_pair(x, y);
  }

  static container_type make(const std::initializer_list<coord_type>& l) {
    return std::pair(l);
  }

  static coord_type& x(container_type& data) { return data.first; }

  static coord_type& y(container_type& data) { return data.second; }
};

template <typename T>
struct underlying_type;

template <typename T>
struct underlying_type<boost::geometry::model::d2::point_xy<T>> {
     using type = T;
};

template <typename T>
using underlying_type_t = typename underlying_type<T>::type;

template <typename T>
struct point_trait<T, typename std::enable_if_t<is_boost_point_v<T>>> {
  using coord_type = underlying_type_t<T>;
  using container_type = T;

  static container_type make(coord_type x, coord_type y) {
    return container_type{x, y};
  }

  static container_type make(const std::initializer_list<coord_type>& l) {
    auto it = std::cbegin(l);
    auto x = *it;
    ++it;
    auto y = *it;
    return container_type{x, y};
  }

  static coord_type& x(container_type& data) { return const_cast<coord_type&>(data.x()); }

  static coord_type& y(container_type& data) { return const_cast<coord_type&>(data.y()); }
};
}  // namespace detail

template <typename T>
struct basic_point {
  using trait = detail::point_trait<T>;
  using coord_type = typename trait::coord_type;
  using container_type = typename trait::container_type;

  container_type data;

  basic_point() = default;
  basic_point(const basic_point&) = default;
  basic_point(basic_point&&) = default;
  basic_point(const std::initializer_list<coord_type>& l)
      : data{trait::make(l)} {}
  basic_point(coord_type x, coord_type y) : data{trait::make(x, y)} {}
  basic_point(container_type c) : data{std::move(c)} {}

  basic_point& operator=(const basic_point&) = default;
  basic_point& operator=(basic_point&&) = default;
  basic_point& operator=(container_type c) {
    data = std::move(c);
    return *this;
  }

  coord_type& x() { return trait::x(data); }
  coord_type& y() { return trait::y(data); }
  const coord_type& x() const {
    return trait::x(const_cast<container_type&>(data));
  }
  const coord_type& y() const {
    return trait::y(const_cast<container_type&>(data));
  }
};

template <typename T>
std::ostream& operator<<(std::ostream& stream, const basic_point<T>& obj) {
  stream << obj.x() << "," << obj.y();
  return stream;
}

template <typename T>
using Point2D = basic_point<std::array<T, 2>>;

template <typename T>
using PointXY = basic_point<T>;

auto PolygonTest() -> void {
  {
    using UIntPoint = Point2D<uint64_t>;
    //using UIntPoint = basic_point<boost::geometry::model::d2::point_xy<uint64_t>>;
    std::vector<UIntPoint> pointArray = {
        {0, 0}, {3, 0}, {1, 4}, {1, 5}, {0, 2}, {0, 0}};
    Polygon<std::vector<UIntPoint>> polygon(pointArray);
    polygon.InPolygonTest(UIntPoint(2, 2));
  }
  {
    using doublePoint = Point2D<double>;
    //using doublePoint = basic_point<boost::geometry::model::d2::point_xy<double>>;
    std::deque<doublePoint> pointArray = {
        {-1, -1}, {3, -2}, {2, 1}, {4, 5}, {2, 3}, {-1, -1}};
    Polygon<std::deque<doublePoint>> polygon(pointArray);
    polygon.InPolygonTest(doublePoint(2, 3));
  }
  {
    using IntPoint = Point2D<int>;
    //using IntPoint = basic_point<boost::geometry::model::d2::point_xy<int64_t>>;
    std::array<IntPoint, 6> pointArray = {
        {{-1, -1}, {3, -2}, {2, 1}, {4, 5}, {-2, 2}, {-1, -1}}};
    Polygon<std::array<IntPoint, 6>> polygon(pointArray);
    polygon.InPolygonTest(IntPoint(2, 3));
  }
  {
    using doublePoint = PointXY<double>;
    //using doublePoint = basic_point<boost::geometry::model::d2::point_xy<double>>;
    std::deque<doublePoint> pointArray = {
        {-2, -2}, {3, -2}, {2, 1}, {4, 5}, {3, 3}, {-2, -2}};
    Polygon<std::deque<doublePoint>> polygon(pointArray);
    polygon.InPolygonTest(doublePoint(2, 3));
  }
  {
//     using IntPoint = Point2D<int>;
//     std::vector<IntPoint> pointArray = {
//       {-2, 2}, {4, 5}, {2, 1}, {3, -2}, {-1, -1} };
//     Polygon<std::vector<IntPoint>> polygon(pointArray);
//     std::vector<IntPoint> toclip = {{1, 5}, {4, 3}, {1, 0}, {0, 0}, {-4, 3} /*{-4, 3}*/}, // TODO:  Is it necessary to close these?
//                           clipped;
    using IntPoint = basic_point<boost::geometry::model::d2::point_xy<int64_t>>;
    std::vector<IntPoint> pointArray = {
        {-1, -1}, {3, -2}, {2, 1}, {4, 5}, {-2, 2}/*, {-1, -1}*/}; // TODO:  These points appear to be needed to be specified in a clockwise orientation.
    Polygon<std::vector<IntPoint>> polygon(pointArray);
    std::vector<IntPoint> toclip = {{-4, 3}, {0, 0}, {1, 0}, {4, 3}, {1, 5}, /*{-4, 3}*/}, // TODO:  Is it necessary to close these?
                          clipped;
    polygon.ClipSegments(toclip, clipped);
  }
  {
    //using IntPoint = Point2D<int>;
    using IntPoint = basic_point<boost::geometry::model::d2::point_xy<int64_t>>;
    std::vector<IntPoint> pointArray = {
        {-1, -1}, {3, -2}, {2, 1}, {4, 5}, {-2, 2}, {-1, -1}};
    Polygon<std::vector<IntPoint>> polygon(pointArray);
    std::vector<IntPoint> toclip = {{0, 1}, {1, 2}, {1, 0}, {0, 0}, {0, 1}},
                          clipped;
    polygon.ClipSegments(toclip, clipped);
  }
  {
    //using IntPoint = Point2D<int>;
    using IntPoint = basic_point<boost::geometry::model::d2::point_xy<int64_t>>;
    std::vector<IntPoint> pointArray = {{0, 0}, {0, 4}, {4, 4}, {4, 0}, {0, 0}};
    Polygon<std::vector<IntPoint>> polygon(pointArray);
    std::vector<IntPoint> toclip = {{2, 2}, {2, 6}, {6, 6}, {6, 2}, {2, 2}}, clipped;
    polygon.ClipSegments(toclip, clipped);
  }
}