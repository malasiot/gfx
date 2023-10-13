#include <gfx/plot/plot.hpp>
#include <gfx/plot/line_plot.hpp>
#include <gfx/plot/bar_chart.hpp>
#include <gfx/plot/error_bars.hpp>

using namespace std ;

namespace gfx {

void Plot::draw(Canvas &c, double w, double h) {

    if ( !x_axis_.has_range_ )
        x_axis_.setRange(data_bounds_.minX(), data_bounds_.maxX()) ;
    if ( !y_axis_.has_range_ )
        y_axis_.setRange(data_bounds_.minY(), data_bounds_.maxY()) ;

    double l = y_axis_.measureWidth(h) ;
    double r = 0 ;
    if ( color_ramp_ )
        r = color_ramp_->measureWidth(h) ;

    double b = x_axis_.measureHeight(w) ;
    double t = 0 ;

    if ( !title_.empty() ) {
        TextLayout layout(title_) ;
        layout.setFont(title_font_) ;
        layout.setWrapWidth(max_title_width_);
        double h = layout.height() ;
        t = h + title_offset_ ;
    }

    double pw = w - l - r - pad_ - pad_;
    double ph = h - t - b - pad_ - pad_;

    if ( aspect_.has_value() ) {
        ph = pw * aspect_.value() ;
    }

    x_axis_.computeLayout(pw) ;
    y_axis_.computeLayout(ph) ;

    c.save() ;
    c.setTransform(Matrix2d::translation(r + pad_, t + ph + pad_));

    x_axis_.draw(c, pw, ph);
    y_axis_.draw(c, pw, ph);

    c.save();
    c.setClipRect(0, 0, pw, -ph) ;
    for( auto &g: graphs_ ) {
        g->draw(c) ;
    }
    c.restore() ;

    if ( !title_.empty() ) {
        c.save() ;
        Rectangle2d layout(pw/2- max_title_width_/2, -ph - title_offset_, max_title_width_, -100 ) ;
        c.setFont(title_font_) ;
        c.setBrush(SolidBrush(NamedColor::black())) ;
        c.setTextAlign(TextAlignHCenter|TextAlignBottom) ;
        c.drawText(title_, layout) ;
        c.restore() ;
    }

    legend_.draw(graphs_, c, pw, ph) ;

    if ( color_ramp_ ) {
        color_ramp_->paint(c, pw, ph) ;
    }

    c.restore() ;

}


LinePlotElement &Plot::lines(const std::vector<double> &x, const std::vector<double> &y, const Pen &pen, Marker *marker) {
    LinePlotElement *g = new LinePlotElement(x, y, pen, marker) ;
    addGraph(g) ;
    return *g ;
}

LinePlotElement &Plot::lines(const std::vector<double> &x, const std::vector<double> &y, const char *spec)
{
    LinePlotElement *g = new LinePlotElement(x, y, spec) ;
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

RasterElement &Plot::raster(const std::vector<double> &data, size_t rows, size_t cols)
{
    RasterElement *g = new RasterElement(data, rows, cols) ;
    addGraph(g) ;
    return *g ;
}

RasterElement &Plot::raster(const std::vector<double> &data, size_t rows, size_t cols, const BoundingBox &ext)
{
    RasterElement *g = new RasterElement(data, rows, cols, ext) ;
    addGraph(g) ;
    return *g ;
}

ColorRamp &Plot::addColorRamp(ColorMap *cmap, double vmin, double vmax) {
    color_ramp_.reset(new ColorRamp(cmap, vmin, vmax)) ;
    return *color_ramp_ ;
}


}
