#include <gfx/bar_chart.hpp>

namespace gfx {

BarGraph &BarPlot::bars(const std::vector<double> &top,  const Color &clr) {
    BarGraph *g = new BarGraph(top, clr) ;
    addGraph(g) ;
    return *g ;
}

BarGraph::BarGraph(const std::vector<double> &top, double width, const Color &clr): top_(top), clr_(clr), width_(width) {

}

BoundingBox BarGraph::getDataBounds() {
    double  miny = numeric_limits<double>::max(),
            maxy = -numeric_limits<double>::max() ;

    for( size_t i = 0 ; i<top_.size() ; i++ ) {
        miny = std::min(miny, top_[i]) ;
        maxy = std::max(maxy, top_[i]) ;
    }

    return { 0, miny, 0, maxy } ;
}

void BarGraph::draw(Canvas &c)
{

}

void BarGraph::drawLegend(Canvas &c, double width, double height)
{

}

}
