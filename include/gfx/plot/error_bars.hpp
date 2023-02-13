#ifndef GFX_PLOT_ERROR_BARS_HPP
#define GFX_PLOT_ERROR_BARS_HPP

#include <gfx/plot/plot_element.hpp>

namespace gfx {

class ErrorBarsElement: public PlotElement {
public:
    ErrorBarsElement(const std::vector<double> &x, const std::vector<double> &y, const std::vector<double> &e) ;

    Pen &pen() { return pen_ ; }

    BoundingBox getDataBounds() override;
    void draw(Canvas &c) override;
    void drawLegend(Canvas &c, double width, double height) override;
    bool hasLegend() const override { return false ; }

private:

    Pen pen_, cap_pen_ ;
    std::vector<double> x_, y_, e_ ;
    uint draw_error_bars_every_nth_ = 1 ;
    double error_bar_cap_width_ = 3.0 ;
} ;

}
#endif

