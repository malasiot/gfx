#pragma once

#include <gfx/plot/axis.hpp>
#include <gfx/plot/colormap.hpp>
#include <gfx/rectangle.hpp>

namespace gfx {
class ColorRamp {
public:
    ColorRamp(ColorMap *cmap, float vmin, float vmax): cmap_(cmap), vmin_(vmin), vmax_(vmax) {
        axis_.setRange(vmin, vmax) ;
        axis_.setTicksPlacement(Axis::TicsInside) ;
        axis_.setDrawMirrorLine(false) ;
        axis_.setMargin(0) ;
    }

    ColorRamp &setFraction(float f) {
        fraction_ = f ; return *this ;
    }

    ColorRamp &setBarWidth(float w) {
        width_ = w ; return *this ;
    }

private:
    friend class Plot ;
    // compute bounding box
    Rectangle2d layout(double h) ;

    void paint(Canvas &p, double w, double h) ;

    double measureWidth(double h) ;

    Image renderBar(size_t w, size_t h) ;

    float fraction_ = 0.7f ;
    float width_ = 20.f ;
    float pad_ = 10.f ;

    ColorMap *cmap_ ;
    float vmin_, vmax_ ;

    Font label_font_ ;

    YAxis axis_ ;
};

}
