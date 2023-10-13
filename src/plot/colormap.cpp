#include <gfx/plot/colormap.hpp>

using namespace std ;

namespace gfx {

LinearSegmentedColormap::LinearSegmentedColormap(const std::vector<Segment> &red, const std::vector<Segment> &green, const std::vector<Segment> &blue):
    red_(red), green_(green), blue_(blue) {
}

double LinearSegmentedColormap::interpolate(const vector<Segment> &seg, double v) {
    for( uint i=0 ; i<seg.size() - 1 ; i++ ) {
        const Segment &seg0 = seg[i] ;
        const Segment &seg1 = seg[i+1] ;
        if ( v < seg0.x_ ) return seg0.y_ ;
        else if ( v < seg1.x_ ) {
            double d = (v - seg0.x_)/(seg1.x_ - seg0.x_) ;
            return (1 - d) * seg0.y_ + d * seg1.y_ ;
        }
    }
    return seg.back().y_ ;
}

Color LinearSegmentedColormap::map(double v) const {
    double r = interpolate(red_, v) ;
    double g = interpolate(green_, v) ;
    double b = interpolate(blue_, v) ;

    return {r, g, b} ;
}

JetColorMap::JetColorMap(): LinearSegmentedColormap({{0., 0}, {0.35, 0}, {0.66, 1}, {0.89, 1}, {1, 0.5}},
                                                    {{0., 0}, {0.125, 0}, {0.375, 1}, {0.64, 1}, {0.91, 0}, {1, 0}},
                                                    {{0., 0.5}, {0.11, 1}, {0.34, 1}, {0.65, 0}, {1, 0}})
{

}

HotColorMap::HotColorMap(): LinearSegmentedColormap({{0., 0.0416}, {0.365079, 1.0}, {1.0, 1.0}},
                                {{0., 0.}, {0.365079, 0.0}, {0.746032, 1.0}, {1.0, 1.0}},
                                {{0., 0.}, {0.746032, 0.0}, {1.0, 1.0}}) {

}

HSVColorMap::HSVColorMap(): LinearSegmentedColormap({{0., 1.}, {0.158730, 1.0}, {0.174603, 0.968750}, {0.333333, 0.031250}, {0.349206, 0.0}, {0.666667, 0.}, {0.682540, 0.031250}, {0.841270, 0.968750},
                     {0.857143, 1.000000},{1.0, 1.0}},
                     {{0., 0.},{0.158730, 0.937500},{0.174603, 1.}, {0.507937, 1.0}, {0.666667, 0.0625}, {0.682540, 0.}, {1.0, 0.}},
                     {{0., 0.}, {0.333333, 0.}, {0.349206, 0.0625}, {0.507937, 1.}, {0.841270, 1.}, {0.857143, 0.9375}, {1.0, 0.09375}}) {
}



} // gfx
