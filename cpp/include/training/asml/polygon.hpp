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

// Potencially needed function
template <typename POINTTYPE>
auto SegmentIntersection(
    const POINTTYPE& seg1start,
    const POINTTYPE& seg1end,
    const POINTTYPE& seg2start,
    const POINTTYPE& seg2end,
    std::vector<POINTTYPE>&
        intersections) {  // TODO: return the intersection(s) of 2 line segments
                          // defined by seg1start, seg2end and seg2start,
                          // seg2end
  // If the line segments overlapped, the 2 end points of the overlapped segment
  // shall be returned

  auto det = (seg1end.x() - seg1start.x()) * (seg2end.y() - seg2start.y()) -
             (seg1end.y() - seg1start.y()) * (seg2end.x() - seg2start.x());

  if (det == 0) {
    // Parallel lines
    return;
  }

  auto t = ((seg2start.x() - seg1start.x()) * (seg2end.y() - seg2start.y()) -
            (seg2start.y() - seg1start.y()) * (seg2end.x() - seg2start.x())) /
           det;
  auto u = ((seg2start.x() - seg1start.x()) * (seg1end.y() - seg1start.y()) -
            (seg2start.y() - seg1start.y()) * (seg1end.x() - seg1start.x())) /
           det;

  if (t >= 0 && t <= 1 && u >= 0 && u <= 1) {
    auto x = seg1start.x() + t * (seg1end.x() - seg1start.x());
    auto y = seg1start.y() + t * (seg1end.y() - seg1start.y());
    intersections.push_back(POINTTYPE(x, y));
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

enum class polygon_type {
    convex,
    complex
};

template <typename POINTARRAY, typename Enabled = void>
struct Polygon {
 public:
  using POINTTYPE = typename POINTARRAY::value_type;
  Polygon(POINTARRAY& points) : _pointArray(points) {}
  virtual ~Polygon() = default;


//   POINTARRY bounding_envelope() const;
//   polygon_type ptype() const;
//   std::vector<Polygon> decompose();
//   void compose(const std::vector<Polygon>& pieces);

  auto InPolygonTest(const POINTTYPE& point) const -> std::string {
    PolygonTestResult ret = PolygonTestResult::UNKNOWN;
    // TODO: implement this function from here and changed the ret to the
    // correct status

    int n = _pointArray.size();
    bool inside = false;

    for (int i = 0, j = n - 1; i < n; j = i++) {
      auto xi = _pointArray[i].x();
      auto yi = _pointArray[i].y();
      auto xj = _pointArray[j].x();
      auto yj = _pointArray[j].y();

      if (((yi > point.y()) != (yj > point.y())) &&
          (point.x() < (xj - xi) * (point.y() - yi) / (yj - yi) + xi)) {
        inside = !inside;
      }

      // Check if the point is on an edge
      if ((point.y() - yi) * (xj - xi) == (point.x() - xi) * (yj - yi) &&
          std::min(xi, xj) <= point.x() && point.x() <= std::max(xi, xj) &&
          std::min(yi, yj) <= point.y() && point.y() <= std::max(yi, yj)) {
        ret = PolygonTestResult::OnPolygonEdge;
        break;
        // return
        // _enumItemStrings[static_cast<int>(PolygonTestResult::OnPolygonEdge)];
      }
    }

    if (ret == PolygonTestResult::UNKNOWN) {
      ret = inside ? PolygonTestResult::InPolygon
                   : PolygonTestResult::OutsidePolygon;
    }

    // return inside ?
    // _enumItemStrings[static_cast<int>(PolygonTestResult::InPolygon)] :
    // _enumItemStrings[static_cast<int>(PolygonTestResult::OutsidePolygon)];

    //////////////////////////////////////////
    std::cout << point << " in polygon ";
    PrintPolygon( _pointArray );
    std::cout << " is " << _enumItemStrings[static_cast<int>(ret)] <<
    std::endl;
    return _enumItemStrings[static_cast<int>(ret)];
  }
  auto ClipSegments(const POINTARRAY& tobeclippedpath, POINTARRAY& clipped)
      -> void {  // tobeclippedpath is the line segment list that adjacent
                 // points forms a line segment
    std::cout << "use ";
    PrintPolygon(_pointArray);
    std::cout << " to clip ";
    PrintPolygon(tobeclippedpath);
    // TODO: implement a function to clip segments and return the clipped
    // segments with 'clipped'


    // Algorithm: (Can the polygon and the clipping polygon have holes?)
    // 1. Get the bounding envelope of both the polygon (precompute at constructor time).
    // 2. Get the bounding envelope of the clipping polygon.
    // 3. If those two envelopes don't intersect there can be no chance the polygons intersect.
    // 4. Decompose the polygon (clip polygon as well) if it is concave into convex pieces (Do this as constructor time?  Can mark it as convex or concave at constructor time.)
    // 5. Perform the clipping on convex polygons only.  Iterate over the polygon pieces and clip against the clip polygon pieces that its bounding envelope intersects.
    // 6. Construct the resulting clip polygon from the clipped convex pieces.


    POINTARRAY output = tobeclippedpath;

    for (size_t i = 0, j = _pointArray.size() - 1; i < _pointArray.size();
         j = i++) {
      POINTARRAY input = output;
      output.clear();

      const POINTTYPE& clipStart = _pointArray[j];
      const POINTTYPE& clipEnd = _pointArray[i];

      POINTTYPE prev = input.back();
      std::vector<POINTTYPE> intersections;

      // Check if the previous point is inside the clipping edge
      bool prevInside = (InPolygonTest(prev) == "InPolygon");

      for (const auto& curr : input) {
        bool currInside = (InPolygonTest(curr) == "InPolygon");

        if (currInside) {
          if (!prevInside) {
            // Find the intersection point
            SegmentIntersection(prev, curr, clipStart, clipEnd, intersections);
            if (!intersections.empty()) {
              output.push_back(
                  intersections
                      .back());  // Assume the last intersection is correct
            }
          }
          output.push_back(curr);
        } else if (prevInside) {
          // Find the intersection point
          SegmentIntersection(prev, curr, clipStart, clipEnd, intersections);
          if (!intersections.empty()) {
            output.push_back(
                intersections
                    .back());  // Assume the last intersection is correct
          }
        }

        prev = curr;
        prevInside = currInside;
      }
    }

    clipped = output;

    std::cout << std::endl << " clipped is";
    PrintPolygon(clipped);
    std::cout << std::endl;
  }

 private:
  POINTARRAY& _pointArray;
};

template <typename POINTARRAY>
struct Polygon<POINTARRAY,
               std::enable_if_t<detail::is_boost_polygon_v<
                   typename POINTARRAY::value_type::container_type>>> {
  using container_type = POINTARRAY;
  using point_type = typename container_type::value_type;
  using boost_type = typename point_type::container_type;

  boost::geometry::model::polygon<boost_type> polygon_;

  Polygon(const container_type& points) : polygon_{[&]() {
    auto pgon = boost::geometry::model::polygon<boost_type>{};
    for (const auto& point : points) {
        boost::geometry::append(pgon.outer(), point.data);
    }
    if (!boost::geometry::is_valid(pgon)) {
        boost::geometry::correct(pgon);
    }
    return pgon;
  }()}{}

  virtual ~Polygon() = default;

  std::string InPolygonTest(const point_type& point) const {
    if (boost::geometry::within(point.data, polygon_)) {
        std::cout << "InPolygon" << std::endl;
        return _enumItemStrings[static_cast<int>(PolygonTestResult::InPolygon)];
    }

    if (boost::geometry::covered_by(point.data, polygon_)) {
        std::cout << "OnPolygonEdge" << std::endl;
        return _enumItemStrings[static_cast<int>(PolygonTestResult::OnPolygonEdge)];
    }

    std::cout << "OutsidePolygon" << std::endl;
    return _enumItemStrings[static_cast<int>(PolygonTestResult::OutsidePolygon)];
  }

  void ClipSegments(const container_type& tobeclippedpath, container_type& clipped) {  
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