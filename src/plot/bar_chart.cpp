#include <gfx/plot/bar_chart.hpp>
#include <gfx/plot/plot.hpp>

#include <numeric>

using namespace std ;

namespace gfx {

BarChartElement::BarChartElement(const std::vector<double> &top, double width, double offset, const Color &clr): top_(top), width_(width) {
    brush_.reset(new SolidBrush(clr)) ;
    x_.resize(top.size()) ;
    bottom_.resize(top.size()) ;

    std::iota(x_.begin(), x_.end(), offset) ;
    std::fill(bottom_.begin(), bottom_.end(), 0.0) ;
}

BarChartElement::BarChartElement(const std::vector<double> &top, const std::vector<double> &bottom, double width, double offset, const Color &clr):
    top_(top), bottom_(bottom), width_(width) {
    brush_.reset(new SolidBrush(clr)) ;
    x_.resize(top.size()) ;

    std::iota(x_.begin(), x_.end(), offset) ;
}

BoundingBox BarChartElement::getDataBounds() {
    double miny = numeric_limits<double>::max(),
            maxy = -numeric_limits<double>::max() ;
    double minx =  miny, maxx = maxy ;

    for( size_t i = 0 ; i<top_.size() ; i++ ) {
        miny = std::min(miny, bottom_[i]) ;
        maxy = std::max(maxy, top_[i]) ;
        minx = std::min(minx, x_[i]) ;
        maxx = std::max(maxx, x_[i]) ;
    }

    return { -width_/2 + minx, miny, maxx + width_/2, maxy } ;
}

void BarChartElement::draw(Canvas &c) {
    auto &xaxis = plot_->xAxis() ;
    auto &yaxis = plot_->yAxis() ;

    c.save() ;
    c.setBrush(*brush_) ;
    if ( pen_ ) c.setPen(*pen_) ;
    else c.clearPen();

    double width = xaxis.getScale() * width_ ;

    for( size_t i=0 ; i<x_.size() ; i++ ) {
        double x = xaxis.transform(x_[i]) ;
        double top = yaxis.transform(top_[i]) ;
        double bottom = yaxis.transform(bottom_[i]) ;
        c.drawRect(x - width/2, top, width, bottom - top);
    }

    c.restore() ;
}

void BarChartElement::drawLegend(Canvas &c, double width, double height) {

    double lh = height/2 ;

    c.save() ;
    c.setBrush(*brush_);
    c.drawRect(0, lh/2, width, lh) ;
    c.restore() ;
}

}
