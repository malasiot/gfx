#ifndef GFX_PLOT_HPP
#define GFX_PLOT_HPP

#include <vector>
#include <gfx/plot/axis.hpp>
#include <gfx/plot/plot_element.hpp>
#include <gfx/plot/legend.hpp>
#include <gfx/plot/line_plot.hpp>
#include <gfx/plot/bar_chart.hpp>
#include <gfx/plot/error_bars.hpp>

namespace gfx {

class Plot: public std::enable_shared_from_this<Plot> {
public:

    Plot() = default;

    Plot &addGraph(PlotElement *g) {
        graphs_.push_back(std::unique_ptr<PlotElement>(g)) ;
        data_bounds_.extend(g->getDataBounds());
        g->plot_ = this ;
        return *this ;
    }

    LinePlotElement &lines(const std::vector<double> &x, const std::vector<double> &y, const char *style = nullptr);

    BarChartElement &bars(const std::vector<double> &top, double width, double offset = 0.0, const Color &clr = NamedColor::blue()) ;
    BarChartElement &bars(const std::vector<double> &top, const std::vector<double> &bottom, double width, double offset = 0.0, const Color &clr = NamedColor::blue()) ;

    ErrorBarsElement &errorbars(const std::vector<double> &x,
                         const std::vector<double> &y,
                         const std::vector<double> &e
                         );

    XAxis &xAxis() { return x_axis_ ; }
    YAxis &yAxis() { return y_axis_ ; }

    Legend &legend() { return legend_ ; }

    Plot &setTitle(const std::string &title) { title_ = title ; return *this ; }

    void draw(Canvas &c, double w, double h) ;

    // show plot on gtk3 window
    void show(const std::string &title = {}, bool blocking = false) ;

private:

    XAxis x_axis_ ;
    YAxis y_axis_ ;
    std::string title_ ;

    Legend legend_ ;

    double max_title_width_ = 200 ;
    double title_offset_ = 4 ;
    Font title_font_ = Font("Arial", 14);

    std::vector<std::unique_ptr<PlotElement>> graphs_ ;
    BoundingBox data_bounds_ ;

private:
    void drawLegend(Canvas &c, double w, double h);
} ;




}

#endif
