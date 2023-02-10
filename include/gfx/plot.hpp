#ifndef GFX_PLOT_HPP
#define GFX_PLOT_HPP

#include <vector>
#include <gfx/axis.hpp>
#include <gfx/graph.hpp>
#include <gfx/legend.hpp>

namespace gfx {

class Graph ;
class LineGraph ;
class Legend ;

class Plot: public std::enable_shared_from_this<Plot> {
public:

    Plot() = default;

    Plot &addGraph(Graph *g) {
        graphs_.push_back(std::unique_ptr<Graph>(g)) ;
        data_bounds_.extend(g->getDataBounds());
        g->plot_ = this ;
        return *this ;
    }

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

    std::vector<std::unique_ptr<Graph>> graphs_ ;
    BoundingBox data_bounds_ ;

private:
    void drawLegend(Canvas &c, double w, double h);
} ;




}

#endif
