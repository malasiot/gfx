#ifndef GFX_PLOT_GRAPH_HPP
#define GFX_PLOT_GRAPH_HPP

#include <gfx/bbox.hpp>
#include <gfx/canvas.hpp>

namespace gfx {
class Plot ;

class PlotElement {
public:

    void setTitle(const std::string &title) { title_ = title ; }
    const std::string &getTitle() const { return title_ ; }

    virtual BoundingBox getDataBounds() = 0;
    virtual void draw(Canvas &c) = 0;
    virtual void drawLegend(Canvas &c, double width, double height) = 0;
    virtual bool hasLegend() const { return true ; }

protected:

    friend class Plot ;

    std::string title_ ;
    Plot *plot_ = nullptr ;
};

}














#endif
