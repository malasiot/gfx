#include <gfx/plot/line_plot.hpp>
#include <gfx/plot/plot.hpp>
#include <gfx/plot/markers.hpp>

using namespace std ;

namespace gfx {

LinePlotElement::LinePlotElement(const vector<double> &x, const vector<double> &y, const char *ps): x_(x), y_(y) {
    assert(x.size() == y.size()) ;
    if ( ps != nullptr ) parseParamString(ps) ;
}

BoundingBox LinePlotElement::getDataBounds() {
    double minx = numeric_limits<double>::max(),
            miny = numeric_limits<double>::max() ,
            maxx = -numeric_limits<double>::max(),
            maxy = -numeric_limits<double>::max() ;

    for( size_t i = 0 ; i<x_.size() ; i++ ) {
        minx = std::min(minx, x_[i]) ;
        miny = std::min(miny, y_[i]) ;
        maxx = std::max(maxx, x_[i]) ;
        maxy = std::max(maxy, y_[i]) ;
    }

    return { minx, miny, maxx, maxy } ;
}

void LinePlotElement::draw(Canvas &c)
{
    auto &xaxis = plot_->xAxis() ;
    auto &yaxis = plot_->yAxis() ;

    // Fill area below graph

    if ( brush_ ) {

        Path p ;

        double x0 = xaxis.transform(x_[0]) ;
        double x1 = xaxis.transform(x_.back()) ;

        p.moveTo(x0, 0) ;

        for( int i=0 ; i<x_.size() ; i++ ) {
            double x = xaxis.transform(x_[i]) ;
            double y = yaxis.transform(y_[i]) ;

            p.lineTo(x, y) ;
        }

        p.lineTo(x1, 0) ;
        p.closePath() ;

        c.save() ;
        c.setBrush(*brush_) ;
        c.clearPen();
        c.drawPath(p) ;
        c.restore() ;

    }

    Path p ;

    for( int i=0 ; i<x_.size() ; i++ ) {
        double x = xaxis.transform(x_[i]) ;
        double y = yaxis.transform(y_[i]) ;

        if ( i == 0 ) p.moveTo(x, y) ;
        else p.lineTo(x, y) ;
    }

    c.save() ;
    c.setPen(pen_) ;
    c.drawPath(p) ;
    c.restore() ;

    if ( marker_ ) {
        for( int i=0 ; i<x_.size() ; i++ ) {
            double x = xaxis.transform(x_[i]) ;
            double y = yaxis.transform(y_[i]) ;

            c.save() ;
            c.setTransform(Matrix2d().translate(x, y)) ;
            marker_->draw(c, i) ;
            c.restore() ;
        }
    }
}

void LinePlotElement::drawLegend(Canvas &c, double width, double height)
{

    double lh = height/2 ;

    c.save() ;
    c.clearPen() ;

    if ( brush_ ) c.setBrush(*brush_);
    else c.clearBrush();

    c.drawRect(0, height/2, width, lh/2) ;
    c.restore() ;

    c.save() ;
    c.setPen(pen_) ;
    c.drawLine(0, height/2.0, width, height/2.0);
    c.restore() ;

    if ( marker_ ) {
        c.save() ;
        c.setTransform(Matrix2d().translate(width/2, height/2)) ;
        marker_->draw(c, 0) ;
        c.restore() ;
    }
}

void LinePlotElement::parseParamString(const char *src) {

    const char *c = src ;
    int markerChar = -1 ;
    int dash = -1  ;

    if ( c == nullptr ) return ;

    Color clr = NamedColor::black() ;

    while (*c != 0 ) {
        switch (*c) {
        case 'y': clr = NamedColor::yellow(); break ; //yellow
        case 'm': clr = NamedColor::magenta() ; break ; // magenta
        case 'c': clr = NamedColor::cyan() ; break ; // cyan
        case 'r': clr = NamedColor::red() ; break ; // red
        case 'g': clr = NamedColor::green() ; break ; // green
        case 'b': clr = NamedColor::blue() ; break ; // blue
        case 'w': clr = NamedColor::white() ; break ;// white
        case 'k': clr = NamedColor::black() ; break ;// black
        case '.': case 'o': case 'x': case '+': case '*':
        case 's': case 'd': case 'v': case '^': case '<':
        case '>': markerChar = *c ; break ;
        case '-': if ( *(c+1) == '-' ) { dash = 2 ; c++ ; } // dashed
            else if ( *(c+1) == '.') { dash = 3 ; c++ ; }// dash-dot
            else { dash = 0 ; } // solid
            break ;
        case ':': dash = 1 ; break ; // dotted
        default: break ;
        }

        c++ ;
    }

    SimpleShapeMarker::Shape shape ;
    switch ( markerChar ) {
    case '.':  shape = SimpleShapeMarker::Point ; break ;
    case 'o':  shape = SimpleShapeMarker::Circle ; break ;
    case 'x':  shape = SimpleShapeMarker::XMark ; break ;
    case '+':  shape = SimpleShapeMarker::Plus ; break ;
    case 's':  shape = SimpleShapeMarker::Square ; break ;
    case '*':  shape = SimpleShapeMarker::Star ; break ;
    case 'd':  shape = SimpleShapeMarker::Diamond ; break ;
    case 'v':  shape = SimpleShapeMarker::TriangleDown ; break ;
    case '^':  shape = SimpleShapeMarker::TriangleUp ; break ;
    case '<':  shape = SimpleShapeMarker::TriangleLeft ; break ;
    case '>':  shape = SimpleShapeMarker::TriangleRight ; break ;
    default:   shape = SimpleShapeMarker::None ; break ;
    }

    setMarker(new SimpleShapeMarker(shape, 6, nullptr, new SolidBrush(NamedColor::white()))) ;

    pen_.setColor(clr) ;

    switch ( dash )
    {
    case 0:
        pen_.setLineStyle(SolidLine) ;
        break ;
    case 1:
        pen_.setLineStyle(DotLine) ;
        break ;
    case 2:
        pen_.setLineStyle(DashLine) ;
        break ;
    case 3:
        pen_.setLineStyle(DashDotLine) ;
        break ;

    }
}


}
