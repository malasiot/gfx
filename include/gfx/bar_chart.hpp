#ifndef GFX_PLOT_BARGRAPH_HPP
#define GFX_PLOT_BARGRAPH_HPP

#include <gfx/graph.hpp>
#include <gfx/plot.hpp>

namespace gfx {

class BarGraph: public Graph {
public:
    BarGraph(const std::vector<double> &top, double width, const Color &clr = NamedColor::blue()) ;
    BarGraph(const std::vector<double> &top, const std::vector<double> &bottom, double w, const Color &clr = NamedColor::blue()) ;

    Pen &pen() { return pen_ ; }
    void setBrush(Brush *brush) {
        brush_.reset(brush) ;
    }

    BoundingBox getDataBounds() override;
    void draw(Canvas &c) override;
    void drawLegend(Canvas &c, double width, double height) override;

private:

    Pen pen_ = Pen() ;
    std::unique_ptr<Brush> brush_  ;
    std::vector<double> top_, bottom_ ;
} ;


class BarPlot: public Plot {
public:
    BarPlot() = default;

    BarGraph &bars(const std::vector<double> &top, const Color &clr =  NamedColor::blue());
} ;

}



#endif
