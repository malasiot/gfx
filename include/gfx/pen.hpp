#ifndef GFX_PEN_HPP
#define GFX_PEN_HPP

#include <gfx/color.hpp>

#include <vector>
#include <memory>

namespace gfx {

enum class LineJoin { Miter, Round, Bevel } ;
enum class LineCap { Butt, Round, Square } ;
enum LineStyle { SolidLine, DashLine, DotLine, DashDotLine, CustomDashLine } ;

class Pen {
public:

    Pen( const Color &clr = Color(0, 0, 0), double line_width = 1.0, LineStyle style = SolidLine ) ;


    Pen &setColor(const Color &brush) ;
    Pen &setLineWidth(double width) ;
    Pen &setMiterLimit(double limit) ;
    Pen &setLineJoin(LineJoin join) ;
    Pen &setLineCap(LineCap cap) ;
    Pen &setDashArray(const std::vector<double> &) ;
    Pen &setDashOffset(double offset) ;
    Pen &setLineStyle(LineStyle dash) ;

    LineJoin lineJoin() const { return line_join_ ; }
    LineCap lineCap() const { return line_cap_ ; }
    LineStyle lineStyle() const { return line_style_ ; }
    Color lineColor() const { return line_color_ ; }
    double lineWidth() const { return line_width_ ; }
    double miterLimit() const { return miter_limit_ ; }
    const std::vector<double> &dashArray() const { return dash_array_ ; }
    double dashOffset() const { return dash_offset_ ; }

private:

    double line_width_, miter_limit_ = 0 ;
    LineJoin line_join_ = LineJoin::Round ;
    LineCap line_cap_ = LineCap::Round ;
    LineStyle line_style_ = SolidLine ;
    std::vector<double> dash_array_ ;
    double dash_offset_ = 0;
    Color line_color_ ;
} ;


}

#endif
