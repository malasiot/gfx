#include <gfx/plot/raster_plot.hpp>
#include <gfx/plot/plot.hpp>

#include <iostream>

using namespace std ;

namespace gfx {
RasterElement::RasterElement(const std::vector<double> &Z, size_t rows, size_t cols): data_(Z), rows_(rows), cols_(cols) {
    xmin_ = -0.5 ; xmax_ = cols_ - 0.5 ;
    ymin_ = -0.5 ; ymax_ = rows_ - 0.5 ;
}

RasterElement::RasterElement(const std::vector<double> &Z, size_t rows, size_t cols, const BoundingBox &extents): data_(Z), rows_(rows), cols_(cols){
    xmin_ = extents.minX() ; ymin_ = extents.minY() ;
    xmax_ = extents.maxX() ; ymax_ = extents.maxY() ;
}


BoundingBox RasterElement::getDataBounds() {
    return { xmin_, ymin_, xmax_, ymax_ } ;
}

void RasterElement::draw(Canvas &c) {

    auto &xaxis = plot_->xAxis() ;
    auto &yaxis = plot_->yAxis() ;

    double x0 = xaxis.transform(xmin_) ;
    double y0 = yaxis.transform(ymin_) ;

    Image image = makeImage() ;

    c.save() ;
    c.setTransform({1.0, 0.0, 0.0, yaxis.isReversed() ? 1.0 : -1.0, x0, y0});
    c.drawImage(image, 1.0);
    c.restore() ;

}

void RasterElement::drawLegend(Canvas &c, double width, double height)
{

}

Color RasterElement::mapColor(double v) const {
    double vn = (v - vmin_)/(vmax_ - vmin_) ;
    Color clr = cmap_->map(vn) ;
    return clr ;
}

static char clip(double v) {
    if ( v < 0 ) return 0 ;
    else if ( v > 255 ) return 255 ;
    else return (char)v ;
}
Image RasterElement::makeImage() {
    computeRange() ;

    auto &xaxis = plot_->xAxis() ;
    auto &yaxis = plot_->yAxis() ;

    double x0 = xaxis.transform(xmin_) ;
    double x1 = xaxis.transform(xmax_) ;

    double x_range = x1 - x0 ;

    double y0 = yaxis.transform(ymin_) ;
    double y1 = yaxis.transform(ymax_) ;
    double y_range = fabs(y0 - y1) ;

    double x_scale = cols_ / x_range ;
    double y_scale = rows_ / y_range ;

    size_t iw = ceil(x_range), ih = ceil(y_range) ;

    Image im(iw, ih, ImageFormat::ARGB32) ;

    for( size_t i=0 ;i<ih ; i++ ) {
        for( size_t j=0 ; j<iw ; j++ ) {

            if ( interpolation_ == RasterInterpolation::Bilinear ) {
                double x = x_scale * j - 0.5;
                double y = y_scale * i - 0.5  ;

                if ( origin_ == RasterOrigin::Upper ) {
                    y = rows_ - 1 - y ;
                }

                int ix0 = floor(x), ix1 = ceil(x) ;
                int iy0 = floor(y), iy1 = ceil(y) ;

                ix0 = std::min(ix0, (int)(cols_-1)) ;
                iy0 = std::min(iy0, (int)(rows_-1)) ;

                ix0 = std::max(ix0, 0) ;
                iy0 = std::max(iy0, 0) ;

                ix1 = std::min(ix1, (int)(cols_-1)) ;
                iy1 = std::min(iy1, (int)(rows_-1)) ;

                ix1 = std::max(ix1, 0) ;
                iy1 = std::max(iy1, 0) ;

                double c00 = data_[iy0 * cols_ + ix0] ;
                double c10 = data_[iy0 * cols_ + ix1] ;
                double c11 = data_[iy1 * cols_ + ix1] ;
                double c01 = data_[iy1 * cols_ + ix0] ;

                double h = x - ix0, v = y - iy0 ;

                double c = (c00 * ( 1 - h ) + c10 * h) * (1 - v) +
                          v * ( c01 * ( 1 - h) + c11 * h ) ;

                Color clr = mapColor(c)  ;

                char *pixel = im.pixels() + + im.stride() * i + 4*j ;

                *pixel++ = 255 ;
                *pixel++ = clip(clr.r() * 255) ;
                *pixel++ = clip(clr.g() * 255) ;
                *pixel++ = clip(clr.b() * 255) ;

            } else {
                double x = x_scale * j ;
                double y = y_scale * i ;

                if ( origin_ == RasterOrigin::Upper ) {
                    y = rows_ - 1 - y ;
                }

                int ix0 = floor(x) ;
                int iy0 = floor(y) ;

                ix0 = std::min(ix0, (int)(cols_-1)) ;
                iy0 = std::min(iy0, (int)(rows_-1)) ;

                ix0 = std::max(ix0, 0) ;
                iy0 = std::max(iy0, 0) ;

                Color clr = mapColor(data_[iy0 * cols_ + ix0])  ;

                char *pixel = im.pixels() + im.stride() * i + 4 * j ;

                *pixel++ = 255 ;
                *pixel++ = clip(clr.r() * 255) ;
                *pixel++ = clip(clr.g() * 255) ;
                *pixel++ = clip(clr.b() * 255) ;
            }
        }
    }


    return im ;
}

void RasterElement::computeRange() {
    vmin_ = vmax_ = data_[0] ;

    for( uint i=0 ;i<rows_ ; i++ ) {
        for( uint j=0 ; j<cols_ ; j++ ) {
            double v = data_[i * cols_ + j] ;
            vmin_ = std::min(vmin_, v) ;
            vmax_ = std::max(vmax_, v) ;
        }
    }
}

}
