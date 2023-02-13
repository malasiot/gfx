#ifndef GFX_PLOT_BAR_CHART_HPP
#define GFX_PLOT_BAR_CHART_HPP

#include <gfx/plot/plot_element.hpp>

namespace gfx {

class BarChartElement: public PlotElement {
public:
    BarChartElement(const std::vector<double> &top, double width, double offset, const Color &clr = NamedColor::blue()) ;
    BarChartElement(const std::vector<double> &top, const std::vector<double> &bottom, double w, double offset, const Color &clr = NamedColor::blue()) ;

    Brush &brush() { return *brush_.get() ; }

    BarChartElement &setBrush(Brush *brush) {
        brush_.reset(brush) ;
        return *this ;
    }

    BarChartElement &setPen(Pen *p) {
        pen_.reset(p) ;
        return *this ;
    }

    BarChartElement &setErrors(const std::vector<double> &e) { e_ = e ; return *this; }

    BoundingBox getDataBounds() override;
    void draw(Canvas &c) override;
    void drawLegend(Canvas &c, double width, double height) override;

private:

    std::unique_ptr<Pen> pen_ ;
    std::unique_ptr<Brush> brush_  ;
    std::vector<double> top_, bottom_, x_, e_ ;
    double width_, offset_ = 0 ;
} ;



}



#endif
