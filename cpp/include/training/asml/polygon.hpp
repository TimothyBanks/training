#pragma once

#include <array>
#include <string>
#include <cstring>
#include <algorithm>
#define POLYGONINOUTSTATUS(code) \
    code(UNKNOWN) code(InPolygon) code(OnPolygonEdge) code(OutsidePolygon)

enum class PolygonTestResult {
#define ENUM_ITEM(x) x,
    POLYGONINOUTSTATUS(ENUM_ITEM)
#undef ENUM_ITEM
    STATECOUNT
};
const std::array<std::string,static_cast<int>(PolygonTestResult::STATECOUNT)> _enumItemStrings = {{
    #define ITEM_STRING(x) #x,
    POLYGONINOUTSTATUS(ITEM_STRING)
    #undef ITEM_STRING
}};

template<typename POINTARRAY>
auto PrintPolygon( const POINTARRAY &polygon) ->void
{
    using POINTTYPE = typename POINTARRAY::value_type;
    std::for_each( polygon.cbegin(), polygon.cend(), [](const POINTTYPE & point ) { std::cout << point << ", ";});
}

// Potencially needed function
template< typename POINTTYPE >
auto SegmentIntersection( const POINTTYPE &seg1start, const POINTTYPE &seg1end, const POINTTYPE &seg2start, const POINTTYPE &seg2end,
                            std::vector<POINTTYPE> &intersections)
{   // TODO: return the intersection(s) of 2 line segments defined by seg1start, seg2end and seg2start, seg2end
    // If the line segments overlapped, the 2 end points of the overlapped segment shall be returned

    auto det = (seg1end.X() - seg1start.X()) * (seg2end.Y() - seg2start.Y()) -
               (seg1end.Y() - seg1start.Y()) * (seg2end.X() - seg2start.X());

    if (det == 0) {
        // Parallel lines
        return;
    }

    auto t = ((seg2start.X() - seg1start.X()) * (seg2end.Y() - seg2start.Y()) -
              (seg2start.Y() - seg1start.Y()) * (seg2end.X() - seg2start.X())) / det;
    auto u = ((seg2start.X() - seg1start.X()) * (seg1end.Y() - seg1start.Y()) -
              (seg2start.Y() - seg1start.Y()) * (seg1end.X() - seg1start.X())) / det;

    if (t >= 0 && t <= 1 && u >= 0 && u <= 1) {
        auto x = seg1start.X() + t * (seg1end.X() - seg1start.X());
        auto y = seg1start.Y() + t * (seg1end.Y() - seg1start.Y());
        intersections.push_back(POINTTYPE(x, y));
    }   
}

template< typename POINTARRAY >
class PolygonT
{
public:
    using POINTTYPE = typename POINTARRAY::value_type;
    PolygonT( POINTARRAY &points ) : _pointArray( points) {}
    virtual ~PolygonT() = default;
    auto InPolygonTest( const POINTTYPE &point ) const -> std::string
    {
        PolygonTestResult ret = PolygonTestResult::UNKNOWN;
        // TODO: implement this function from here and changed the ret to the correct status


        int n = _pointArray.size();
        bool inside = false;

        for (int i = 0, j = n - 1; i < n; j = i++) {
            auto xi = _pointArray[i].X();
            auto yi = _pointArray[i].Y();
            auto xj = _pointArray[j].X();
            auto yj = _pointArray[j].Y();

            if (((yi > point.Y()) != (yj > point.Y())) &&
                (point.X() < (xj - xi) * (point.Y() - yi) / (yj - yi) + xi)) {
                inside = !inside;
            }

            // Check if the point is on an edge
            if ((point.Y() - yi) * (xj - xi) == (point.X() - xi) * (yj - yi) &&
                std::min(xi, xj) <= point.X() && point.X() <= std::max(xi, xj) &&
                std::min(yi, yj) <= point.Y() && point.Y() <= std::max(yi, yj)) {
                ret = PolygonTestResult::OnPolygonEdge;
                break;
                // return _enumItemStrings[static_cast<int>(PolygonTestResult::OnPolygonEdge)];
            }
        }

        if (ret == PolygonTestResult::UNKNOWN) {
            ret = inside ? PolygonTestResult::InPolygon : PolygonTestResult::OutsidePolygon;
        }

        // return inside ? _enumItemStrings[static_cast<int>(PolygonTestResult::InPolygon)] : _enumItemStrings[static_cast<int>(PolygonTestResult::OutsidePolygon)];





        //////////////////////////////////////////
        // std::cout << point << " in polygon ";
        // PrintPolygon( _pointArray );
        // std::cout << " is " << _enumItemStrings[static_cast<int>(ret)] << std::endl;
        return _enumItemStrings[static_cast<int>(ret)];
    }
    auto ClipSegments( const POINTARRAY &tobeclippedpath, POINTARRAY &clipped ) -> void
    {   // tobeclippedpath is the line segment list that adjacent points forms a line segment
        std::cout << "use ";
        PrintPolygon( _pointArray );
        std::cout << " to clip ";
        PrintPolygon(tobeclippedpath);
        // TODO: implement a function to clip segments and return the clipped segments with 'clipped'


POINTARRAY output = tobeclippedpath;

    for (size_t i = 0, j = _pointArray.size() - 1; i < _pointArray.size(); j = i++) {
        POINTARRAY input = output;
        output.clear();

        const POINTTYPE &clipStart = _pointArray[j];
        const POINTTYPE &clipEnd = _pointArray[i];

        POINTTYPE prev = input.back();
        std::vector<POINTTYPE> intersections;

        // Check if the previous point is inside the clipping edge
        bool prevInside = (InPolygonTest(prev) == "InPolygon");

        for (const auto &curr : input) {
            bool currInside = (InPolygonTest(curr) == "InPolygon");

            if (currInside) {
                if (!prevInside) {
                    // Find the intersection point
                    SegmentIntersection(prev, curr, clipStart, clipEnd, intersections);
                    if (!intersections.empty()) {
                        output.push_back(intersections.back()); // Assume the last intersection is correct
                    }
                }
                output.push_back(curr);
            } else if (prevInside) {
                // Find the intersection point
                SegmentIntersection(prev, curr, clipStart, clipEnd, intersections);
                if (!intersections.empty()) {
                    output.push_back(intersections.back()); // Assume the last intersection is correct
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
    POINTARRAY &_pointArray;
};