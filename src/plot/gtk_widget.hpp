#pragma once

#include <gtkmm.h>
#include <gfx/plot.hpp>

namespace gfx {

class PlotArea : public Gtk::DrawingArea
{
public:
    PlotArea(std::shared_ptr<Plot> plot): plot_(plot) {}
    virtual ~PlotArea(){}

    void setPlot(const std::shared_ptr<Plot> &plot) ;

    void setMargin(double margin) {
        margin_ = margin ;
    }

protected:
    bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;

    std::shared_ptr<Plot> plot_ ;
    double margin_ = 4 ;
};
}
