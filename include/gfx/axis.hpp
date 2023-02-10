#ifndef GFX_PLOT_AXIS_HPP
#define GFX_PLOT_AXIS_HPP

#include <string>
#include <memory>

#include <gfx/canvas.hpp>
#include <gfx/tics.hpp>

namespace gfx {

class Axis {
public:

    enum TicsPlacement { TicsInside, TicsOutside } ;

    Axis() {
        grid_pen_.setLineWidth(0.2) ;
    }

    Axis &setRange(double min_v, double max_v) {
        min_v_ = min_v ; max_v_ = max_v ;
        has_range_ = true ;
        return *this ;
    }

    Axis &setTitle(const std::string &title) { title_ = title ; return *this ; }
    Axis &setTickFormatter(TickFormatter *tf) { tick_formatter_.reset(tf) ; return *this ; }
    Axis &setLogarithmic(bool loga) { is_log_ = loga ; return *this ; }
    Axis &setGrid(bool grid) { draw_grid_ = grid ; return *this ; }
    Axis &setTickLocator(TickLocator *loc) { tick_locator_.reset(loc) ; return *this ;}

    Axis &setLabelSeparation(double s) { label_sep_ = s ; return *this ; }
    Axis &setLabelFont(const Font &f) { label_font_ = f ; return *this ; }
    Axis &setLabelOffset(double o) { label_offset_ = 0 ; return *this ; }

    double getScale() const { return scale_ ; }
    double getOffset() const { return offset_ ; }
    bool isLogarithmic() const { return is_log_ ; }

protected:

    // compute tic positions
    void computeAxisLayout(double ls, double wsize);

    Rectangle2d paintLabel(Canvas &c, const std::string &label, double x, double y, double ox, double oy) ;

protected:

    friend class Plot ;

    double margin_ = 12.0 ;
    double label_sep_ = 20 ;
    bool is_log_ = false ;
    bool is_reversed_ = false ;
    double tic_size_ = 5 ;
    double tic_minor_size_ = 3 ;
    double label_offset_ = 5 ;
    double title_offset_ = 5 ;
    double title_wrap_ = 100 ;
    bool draw_grid_ = false ;
    std::string title_ ;
    std::unique_ptr<TickFormatter> tick_formatter_ = std::unique_ptr<TickFormatter>(new ScalarTickFormatter()) ;
    std::unique_ptr<TickLocator> tick_locator_ = std::unique_ptr<TickLocator>(new AutoTickLocator()) ;

    TicsPlacement tics_placement_ = TicsOutside ;

    Font label_font_ = Font("Arial", 14) ;
    Font title_font_ = Font("Arial", 16) ;
    SolidBrush text_brush_ = SolidBrush(NamedColor::black()) ;
    Pen tics_pen_ = Pen(), grid_pen_ ;

    // layout data

    double step_, ls_ ;
    double vscale_, power_ ; // scaled applied to the displayed label values
    double min_v_, max_v_ ;
    double min_label_v_, max_label_v_ ; // minimum/maximum displayed label value
    double scale_, offset_  ;
    std::vector<std::string> labels_ ;
    std::vector<double> tic_locations_ ;
    bool has_range_ = false ;

};

class XAxis: public Axis {
public:

    void computeLayout(double wsize);

    void draw(Canvas &canvas, double wsize, double hsize);

    // from data coordinates to device coordinates
    double transform(double x) ;
};

class YAxis: public Axis {
public:

    void computeLayout(double wsize);
    void draw(Canvas &canvas, double wsize, double hsize);

    // from data coordinates to device coordinates
    double transform(double x) ;
};

}

#endif
