#include <gfx/plot/error_bars.hpp>
#include <gfx/plot/plot.hpp>

using namespace std ;

namespace gfx {

ErrorBarsElement::ErrorBarsElement(const std::vector<double> &x, const std::vector<double> &y, const std::vector<double> &e):
    x_(x), y_(y), e_(e) {
}

BoundingBox ErrorBarsElement::getDataBounds() {
    double minx = numeric_limits<double>::max(),
            miny = numeric_limits<double>::max() ,
            maxx = -numeric_limits<double>::max(),
            maxy = -numeric_limits<double>::max() ;

    for( size_t i = 0 ; i<x_.size() ; i++ ) {
        minx = std::min(minx, x_[i]) ;
        miny = std::min(miny, y_[i] - e_[i]) ;
        maxx = std::max(maxx, x_[i]) ;
        maxy = std::max(maxy, y_[i] + e_[i]) ;
    }

    return { minx, miny, maxx, maxy } ;
}

void ErrorBarsElement::draw(Canvas &c)
{
    auto &xaxis = plot_->xAxis() ;
    auto &yaxis = plot_->yAxis() ;

    for( int i=0 ; i<x_.size() ; i+=draw_error_bars_every_nth_ ) {
        double x = xaxis.transform(x_[i]) ;
        double e =  e_[i];

        double y1 = yaxis.transform(y_[i] - e) ;
        double y2 = yaxis.transform(y_[i] + e) ;
        c.save() ;
        c.setPen(pen_) ;
        c.drawLine(x, y1, x, y2) ;
        c.restore() ;

        c.save() ;
        c.setPen(cap_pen_) ;
        c.drawLine(x -error_bar_cap_width_/2, y1, x+error_bar_cap_width_/2, y1) ;
        c.drawLine(x -error_bar_cap_width_/2, y2, x+error_bar_cap_width_/2, y2) ;
        c.restore() ;

    }

}

void ErrorBarsElement::drawLegend(Canvas &c, double width, double height) {

}


}
