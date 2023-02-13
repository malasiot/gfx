#include <gfx/plot/plot.hpp>
#include <gfx/plot/line_plot.hpp>
#include <gfx/plot/bar_chart.hpp>
#include <gfx/plot/error_bars.hpp>

using namespace std ;

namespace gfx {

void Plot::draw(Canvas &c, double w, double h)
{
    if ( !x_axis_.has_range_ )
        x_axis_.setRange(data_bounds_.minX(), data_bounds_.maxX()) ;
    if ( !y_axis_.has_range_ )
        y_axis_.setRange(data_bounds_.minY(), data_bounds_.maxY()) ;

    x_axis_.computeLayout(w) ;
    y_axis_.computeLayout(h) ;

    x_axis_.draw(c, w, h);
    y_axis_.draw(c, w, h);

    c.drawLine(0, -h, w, -h) ;
    c.drawLine(w, 0, w, -h) ;

    c.save();
    c.setClipRect(0, 0, w, -h) ;
    for( auto &g: graphs_ ) {
        g->draw(c) ;
    }
    c.restore() ;

    if ( !title_.empty() ) {
        c.save() ;
        Rectangle2d layout(w/2- max_title_width_/2, -h - title_offset_, max_title_width_, -100 ) ;
        c.setFont(title_font_) ;
        c.setBrush(SolidBrush(NamedColor::black())) ;
        c.setTextAlign(TextAlignHCenter|TextAlignBottom) ;
        c.drawText(title_, layout) ;
        c.restore() ;
    }

    legend_.draw(graphs_, c, w, h) ;

}


LinePlotElement &Plot::lines(const std::vector<double> &x, const std::vector<double> &y, const char *style) {
    LinePlotElement *g = new LinePlotElement(x, y, style) ;
    addGraph(g) ;
    return *g ;
}

BarChartElement &Plot::bars(const std::vector<double> &top, double w, double offset, const Color &clr) {
    BarChartElement *g = new BarChartElement(top, w, offset, clr) ;
    addGraph(g) ;
    return *g ;
}

BarChartElement &Plot::bars(const std::vector<double> &top, const std::vector<double> &bottom, double width, double offset, const Color &clr) {
    BarChartElement *g = new BarChartElement(top, bottom, width, offset, clr) ;
    addGraph(g) ;
    return *g ;
}

ErrorBarsElement &Plot::errorbars(const std::vector<double> &x, const std::vector<double> &y, const std::vector<double> &e) {
    ErrorBarsElement *g = new ErrorBarsElement(x, y, e) ;
    addGraph(g) ;
    return *g ;
}


}
