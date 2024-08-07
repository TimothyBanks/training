#pragma once

#include <algorithm>
#include <array>
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/polygon.hpp>
#include <cstring>
#include <string>

#define POLYGONINOUTSTATUS(code) \
  code(UNKNOWN) code(InPolygon) code(OnPolygonEdge) code(OutsidePolygon)

enum class PolygonTestResult {
#define ENUM_ITEM(x) x,
  POLYGONINOUTSTATUS(ENUM_ITEM)
#undef ENUM_ITEM
      STATECOUNT
};
const std::array<std::string, static_cast<int>(PolygonTestResult::STATECOUNT)>
    _enumItemStrings = {{
#define ITEM_STRING(x) #x,
        POLYGONINOUTSTATUS(ITEM_STRING)
#undef ITEM_STRING
    }};

template <typename POINTARRAY>
auto PrintPolygon(const POINTARRAY& polygon) -> void {
  using POINTTYPE = typename POINTARRAY::value_type;
  std::for_each(std::cbegin(polygon), std::cend(polygon),
                [](const POINTTYPE& point) { std::cout << point << ", "; });
}

using float32 = float;
using float64 = double;

template <typename POINTTYPE>
typename POINTTYPE::coord_type cross_product(const POINTTYPE& a,
                                             const POINTTYPE& b) {
  return a.x() * b.y() - a.y() * b.x();
}

template <typename POINTTYPE>
int8_t orientation(const POINTTYPE& p, const POINTTYPE& q, const POINTTYPE& r) {
  double val =
      (q.y() - p.y()) * (r.x() - q.x()) - (q.x() - p.x()) * (r.y() - q.y());
  if (val == 0)
    return 0;                // Collinear
  return (val > 0) ? 1 : 2;  // Clockwise or Counterclockwise
}

template <typename POINTTYPE>
bool are_collinear(const POINTTYPE& a, const POINTTYPE& b, const POINTTYPE& c) {
  auto ab = POINTTYPE{b.x() - a.x(), b.y() - a.y()};
  auto ac = POINTTYPE{c.x() - a.x(), c.y() - a.y()};
  return cross_product(ab, ac) == 0;
}

template <typename POINTTYPE>
typename POINTTYPE::coord_type distance(const POINTTYPE& a,
                                        const POINTTYPE& b) {
  return (b.x() - a.x()) * (b.x() - a.x()) + (b.y() - a.y()) * (b.y() - a.y());
}

template <typename POINTTYPE>
bool on_segment(const POINTTYPE& p, const POINTTYPE& q, const POINTTYPE& r) {
  auto dpq = static_cast<float64>(distance(p, q));
  auto dpr = static_cast<float64>(distance(p, r));
  auto dqr = static_cast<float64>(distance(q, r));

  static constexpr auto epsilon = 1e-9;
  return std::abs(dpq + dpr - dqr) < epsilon;
}

template <typename POINTTYPE>
std::pair<const POINTTYPE*, const POINTTYPE*> find_overlap(const POINTTYPE& a,
                                                           const POINTTYPE& b,
                                                           const POINTTYPE& c,
                                                           const POINTTYPE& d) {
  auto points = std::vector<const POINTTYPE*>{&a, &b, &c, &d};
  std::sort(std::begin(points), std::end(points),
            [](const POINTTYPE* p1, const POINTTYPE* p2) {
              return p1->x() < p2->x() ||
                     (p1->x() == p2->x() && p1->y() < p2->y());
            });
  return {points[1], points[2]};
}

// Potentially needed function
template <typename POINTTYPE>
auto SegmentIntersection(const POINTTYPE& seg1start,
                         const POINTTYPE& seg1end,
                         const POINTTYPE& seg2start,
                         const POINTTYPE& seg2end,
                         std::vector<POINTTYPE>& intersections) {
  // If the line segments overlapped, the 2 end points of the overlapped segment
  // shall be returned

  // Determine the orientation of each triplet
  auto o1 = orientation(seg1start, seg1end, seg2start);
  auto o2 = orientation(seg1start, seg1end, seg2end);
  auto o3 = orientation(seg2start, seg2end, seg1start);
  auto o4 = orientation(seg2start, seg2end, seg1end);

  // General case
  if (o1 != o2 && o3 != o4) {
    // Line AB represented as a1*x + b1*y = c1
    auto a1 = seg1end.y() - seg1start.y();
    auto b1 = seg1start.x() - seg1end.x();
    auto c1 = a1 * seg1start.x() + b1 * seg1start.y();

    // Line CD represented as a2*x + b2*y = c2
    auto a2 = seg2end.y() - seg2start.y();
    auto b2 = seg2start.x() - seg2end.x();
    auto c2 = a2 * seg2start.x() + b2 * seg2start.y();

    auto determinant = a1 * b2 - a2 * b1;

    if (determinant != 0) {
      auto x = (b2 * c1 - b1 * c2) / determinant;
      auto y = (a1 * c2 - a2 * c1) / determinant;
      intersections.push_back({x, y});
    }
    return;  // Intersection at a unique point
  }

  // Special cases: Check for collinear points and overlapping segments
  auto result = std::vector<POINTTYPE>{};
  if (o1 == 0 && on_segment(seg2start, seg1start, seg1end))
    result.push_back(seg2start);
  if (o2 == 0 && on_segment(seg2end, seg1start, seg1end))
    result.push_back(seg2end);
  if (o3 == 0 && on_segment(seg1start, seg2start, seg2end))
    result.push_back(seg1start);
  if (o4 == 0 && on_segment(seg1end, seg2start, seg2end))
    result.push_back(seg1end);

  // If the segments are collinear and overlapping, return the overlap
  if (result.size() > 1) {
    // Sort the points by x, then by y to find the overlapping segment
    std::sort(std::begin(result), std::end(result),
              [](const POINTTYPE& p1, const POINTTYPE& p2) { return p1 < p2; });
    // Return the endpoints of the overlapping segment
    intersections.push_back(result.front());
    intersections.push_back(result.back());
    return;
  }

  for (auto& p : result) {
    intersections.push_back(std::move(p));
  }
}

namespace detail {

template <typename T>
struct is_boost_polygon : std::false_type {};

template <typename T>
struct is_boost_polygon<boost::geometry::model::d2::point_xy<T>>
    : std::true_type {};

template <typename T>
static constexpr auto is_boost_polygon_v = is_boost_polygon<T>::value;

}  // namespace detail

template <typename POINTARRAY, typename Enabled = void>
struct Polygon {
  using POINTTYPE = typename POINTARRAY::value_type;

  const POINTARRAY& point_array_;

  Polygon(const POINTARRAY& points) : point_array_{points} {
    if (point_array_.size() < 3) {
      throw "Polygon must contain at least 3 points.";
    }
  }

  std::string InPolygonTest(const POINTTYPE& p) const {
    auto ret = PolygonTestResult::UNKNOWN;
    auto count = size_t{0};
    auto n = point_array_.size();

    for (int i = 0; i < n; ++i) {
      auto a = point_array_[i];
      auto b = point_array_[(i + 1) % n];

      // Check if point is exactly on the edge
      if (are_collinear(p, a, b)) {
        if (on_segment(p, a, b)) {
          ret = PolygonTestResult::OnPolygonEdge;
          break;
        }
      }

      // Count intersections of the ray from p to the right with the edges of
      // the polygon
      if (a.y() <= p.y() && b.y() > p.y() || b.y() <= p.y() && a.y() > p.y()) {
        auto intersect_x =
            a.x() + (p.y() - a.y()) / (b.y() - a.y()) * (b.x() - a.x());
        if (p.x() < intersect_x) {
          ++count;
        }
      }
    }

    // Odd count means inside, even means outside
    if (ret == PolygonTestResult::UNKNOWN) {
      ret = count % 2 == 1 ? PolygonTestResult::InPolygon
                           : PolygonTestResult::OutsidePolygon;
    }

    //////////////////////////////////////////
    // std::cout << p << " in polygon ";
    // PrintPolygon( point_array_ );
    // std::cout << " is " << _enumItemStrings[static_cast<int>(ret)] <<
    // std::endl;
    std::cout << _enumItemStrings[static_cast<int>(ret)] << std::endl;
    return _enumItemStrings[static_cast<int>(ret)];
  }

  auto ClipSegments(const POINTARRAY& tobeclippedpath, POINTARRAY& clipped)
      -> void {  // tobeclippedpath is the line segment list that adjacent
                 // points forms a line segment
    // std::cout << "use ";
    // PrintPolygon(point_array_);
    // std::cout << " to clip ";
    // PrintPolygon(tobeclippedpath);
    // TODO: implement a function to clip segments and return the clipped
    // segments with 'clipped'

    // Algorithm: (Can the polygon and the clipping polygon have holes?)
    // 1. Get the bounding envelope of both the polygon (precompute at
    // constructor time).
    // 2. Get the bounding envelope of the clipping polygon.
    // 3. If those two envelopes don't intersect there can be no chance the
    // polygons intersect.
    // 4. Decompose the polygon (clip polygon as well) if it is concave into
    // convex pieces (Do this as constructor time?  Can mark it as convex or
    // concave at constructor time.)
    // 5. Perform the clipping on convex polygons only.  Iterate over the
    // polygon pieces and clip against the clip polygon pieces that its bounding
    // envelope intersects.
    // 6. Construct the resulting clip polygon from the clipped convex pieces.

    // loop through all the segments in tobeclippedpath
    for (auto i = 0; i < tobeclippedpath.size(); ++i) {
      auto start_point = tobeclippedpath[i];
      auto end_point = tobeclippedpath[(i + 1) % tobeclippedpath.size()];
      //   auto start_status = InPolygonTest(start_point);
      //   auto end_status = InPolygonTest(end_point);
      //   if (start_status == "InPolygon" && end_status == "InPolygon" ||
      //       start_status == "OnPolygonEdge" && end_status == "OnPolygonEdge")
      //       {
      //     clipped.push_back(start_point);
      //     clipped.push_back(end_point);
      //     continue;
      //   }

      for (auto j = 0; j < point_array_.size(); ++j) {
        auto p = point_array_[j];
        auto q = point_array_[(j + 1) % point_array_.size()];
        SegmentIntersection(p, q, start_point, end_point, clipped);
      }
      // if one point is inside the polygon and another is outside, we need to
      // add in the inside point
      //   if (start_status == "InPolygon") {
      //     clipped.push_back(start_point);
      //   }
      //   if (end_status == "InPolygon") {
      //     clipped.push_back(end_point);
      //   }
    }

    std::cout << std::endl << " clipped is ";
    PrintPolygon(clipped);
    std::cout << std::endl;
  }
};

template <typename POINTARRAY>
struct Polygon<POINTARRAY,
               std::enable_if_t<detail::is_boost_polygon_v<
                   typename POINTARRAY::value_type::container_type>>> {
  using container_type = POINTARRAY;
  using point_type = typename container_type::value_type;
  using boost_type = typename point_type::container_type;

  boost::geometry::model::polygon<boost_type> polygon_;

  Polygon(const container_type& points)
      : polygon_{[&]() {
          auto pgon = boost::geometry::model::polygon<boost_type>{};
          for (const auto& point : points) {
            boost::geometry::append(pgon.outer(), point.data);
          }
          if (!boost::geometry::is_valid(pgon)) {
            boost::geometry::correct(pgon);
          }
          return pgon;
        }()} {}

  virtual ~Polygon() = default;

  std::string InPolygonTest(const point_type& point) const {
    if (boost::geometry::within(point.data, polygon_)) {
      std::cout << "InPolygon" << std::endl;
      return _enumItemStrings[static_cast<int>(PolygonTestResult::InPolygon)];
    }

    if (boost::geometry::covered_by(point.data, polygon_)) {
      std::cout << "OnPolygonEdge" << std::endl;
      return _enumItemStrings[static_cast<int>(
          PolygonTestResult::OnPolygonEdge)];
    }

    std::cout << "OutsidePolygon" << std::endl;
    return _enumItemStrings[static_cast<int>(
        PolygonTestResult::OutsidePolygon)];
  }

  void ClipSegments(const container_type& tobeclippedpath,
                    container_type& clipped) {
    auto clip = Polygon<container_type>{tobeclippedpath};
    auto output = std::vector<boost::geometry::model::polygon<boost_type>>{};

    boost::geometry::intersection(polygon_, clip.polygon_, output);
    if (!output.empty()) {
      for (auto point : output.front().outer()) {
        clipped.push_back(std::move(point));
      }
    }

    std::cout << std::endl << " clipped is ";
    PrintPolygon(clipped);
    std::cout << std::endl;
  }
};