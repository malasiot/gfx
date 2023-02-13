#ifndef GFX_PLOT_LINEPLOT_HPP
#define GFX_PLOT_LINEPLOT_HPP

#include <gfx/plot/plot_element.hpp>
#include <gfx/plot/markers.hpp>


namespace gfx {

class LinePlotElement: public PlotElement {
public:
    LinePlotElement(const std::vector<double> &x, const std::vector<double> &y, const char *ps = nullptr) ;

    Pen &pen() { return pen_ ; }

    LinePlotElement & setMarker(Marker *marker) { marker_.reset(marker) ; return *this ; }
    void setBrush(Brush *brush) {
        brush_.reset(brush) ;
    }

    BoundingBox getDataBounds() override;
    void draw(Canvas &c) override;
    void drawLegend(Canvas &c, double width, double height) override;

    void parseParamString(const char *ps) ;

private:

    Pen pen_ = Pen() ;
    std::unique_ptr<Brush> brush_  ;
    std::vector<double> x_, y_, e_ ;
    std::unique_ptr<Marker> marker_ ;
} ;

}
#endif

