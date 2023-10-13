#pragma once

#include <gfx/color.hpp>
#include <vector>

namespace gfx {

class ColorMap {
public:
    virtual Color map(double v) const = 0;
};



class GrayScaleColorMap: public ColorMap {
public:
    Color map(double v) const override {
        return Color{1.0-v, 1.0-v, 1.0-v} ;
    }
};

class LinearSegmentedColormap: public ColorMap {
protected:
    struct Segment { double x_, y_ ;  } ;
public:
    LinearSegmentedColormap(const std::vector<Segment> &red, const std::vector<Segment> &green, const std::vector<Segment> &blue) ;

    Color map(double v) const override ;

private:


    static double interpolate(const std::vector<Segment> &s, double x) ;

    std::vector<Segment> red_, green_, blue_ ;
};

class JetColorMap: public LinearSegmentedColormap {
public:
    JetColorMap() ;
};

class HotColorMap: public LinearSegmentedColormap {
public:
    HotColorMap() ;
};

class HSVColorMap: public LinearSegmentedColormap {
public:
    HSVColorMap() ;
};
}
