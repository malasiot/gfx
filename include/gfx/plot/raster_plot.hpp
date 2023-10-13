#pragma once

#include <gfx/plot/plot_element.hpp>
#include <gfx/plot/colormap.hpp>

namespace gfx {


enum class RasterInterpolation {
    Bilinear, Flat
};

enum class RasterOrigin {
    Lower, Upper
};

class RasterElement: public PlotElement {
public:

    RasterElement(const std::vector<double> &data, size_t rows, size_t cols) ;
    RasterElement(const std::vector<double> &data, size_t rows, size_t cols, const BoundingBox &extents) ;

    RasterElement &setRange(double vmin, double vmax) {
        vmin_ = vmin ;
        vmax_ = vmax ;
        has_range_ = true ;
        return *this ;
    }


    RasterElement &setColorMap(ColorMap *cmap) {
        cmap_.reset(cmap) ;
        return *this ;
    }

    RasterElement &setInterpolation(RasterInterpolation i) {
        interpolation_ = i ;
        return *this ;
    }


    RasterElement &setOrigin(RasterOrigin o) {
        origin_ = o ;
        return *this ;
    }

    RasterElement &showColorRamp(bool s) {
        show_color_ramp_ = s ;
        return *this ;
    }

    BoundingBox getDataBounds() override;
    void draw(Canvas &c) override;
    void drawLegend(Canvas &c, double width, double height) override;

private:

    Image makeImage() ;
    void computeRange() ;
    Color mapColor(double v) const;

    std::unique_ptr<ColorMap> cmap_{new GrayScaleColorMap()}  ;
    std::unique_ptr<Image> image_ ;
    std::vector<double> data_ ;
    size_t rows_, cols_ ;
    double vmin_, vmax_;
    double xmin_, ymin_, xmax_, ymax_ ;
    bool has_range_ = false, show_color_ramp_ = false ;
    RasterInterpolation interpolation_ = RasterInterpolation::Bilinear;
    RasterOrigin origin_ = RasterOrigin::Upper ;
} ;

}


