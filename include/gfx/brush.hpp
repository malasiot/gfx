#ifndef GFX_BRUSH_HPP
#define GFX_BRUSH_HPP

#include <gfx/color.hpp>
#include <gfx/xform.hpp>
#include <gfx/surface.hpp>

#include <vector>
#include <memory>


namespace gfx {

enum class FillRule { EvenOdd, NonZero } ;

// abstract class for all brush type

class Brush {
public:

    virtual ~Brush() = default;
    virtual Brush *clone() const = 0 ;

    void setFillRule(FillRule rule) { fill_rule_ = rule ; }
    void setFillOpacity(double opacity) { fill_opacity_ = opacity; }

    double fillOpacity() const { return fill_opacity_ ; }
    FillRule fillRule() const { return fill_rule_ ; }


protected:

    Brush(): fill_rule_(FillRule::EvenOdd), fill_opacity_(1.0) {}

public:

    FillRule fill_rule_ ;
    double fill_opacity_ ;
} ;


class SolidBrush: public Brush {

public:

    SolidBrush(const Color &clr): clr_(clr) {}

    const Color color() const { return clr_ ; }

    Brush *clone() const override { return new SolidBrush(*this) ; }

private:

    Color clr_ ;
} ;

enum class SpreadMethod { Pad, Repeat, Reflect } ;

class GradientBrush: public Brush {

public:

    void setSpread(SpreadMethod method) { sm_ = method ; }
    void setTransform(const Matrix2d &trans) { tr_ = trans ; }

    struct Stop {
        Stop(double offset, const Color &clr): offset_(offset), clr_(clr) {}
        double offset_ ;
        Color clr_ ;
    } ;

    GradientBrush &addStop(double offset, const Color &clr) {
        stops_.push_back(Stop(offset, clr)) ;
        return *this ;
    }


    SpreadMethod spread() const { return sm_ ; }
    const Matrix2d &transform() const { return tr_ ; }
    const std::vector<Stop> &stops() const { return stops_ ; }

    Brush *clone() const  override { return new GradientBrush(*this) ; }

protected:

    GradientBrush(): sm_(SpreadMethod::Pad) {}

private:

    std::vector<Stop> stops_ ;
    SpreadMethod sm_ ;
    Matrix2d tr_ ;
} ;


class LinearGradientBrush: public GradientBrush {

public:

    LinearGradientBrush(double x0, double y0, double x1, double y1):
        x0_(x0), y0_(y0), x1_(x1), y1_(y1) {}

    Brush *clone() const override { return new LinearGradientBrush(*this) ; }

    double x0() const { return x0_ ; }
    double y0() const { return y0_ ; }
    double x1() const { return x1_ ; }
    double y1() const { return y1_ ; }


private:

    double x0_, y0_, x1_, y1_ ;
} ;

class RadialGradientBrush: public GradientBrush {

public:

    RadialGradientBrush(double cx, double cy, double r, double fx = 0, double fy = 0):
        cx_(cx), cy_(cy), fx_(fx), fy_(fy), r_(r)  {}

    Brush *clone() const override { return new RadialGradientBrush(*this) ; }

    double cx() const { return cx_ ; }
    double cy() const { return cy_ ; }
    double fx() const { return fx_ ; }
    double fy() const { return fy_ ; }
    double radius() const { return r_ ; }

private:

    double cx_, cy_, fx_, fy_, r_ ;
} ;

class Canvas ;

class PatternBrush: public Brush {

public:

    PatternBrush(const Surface &surf) ;

    Brush *clone() const override {
        return new PatternBrush(*this) ;
    }

    cairo_pattern_t *handle() const { return pattern_.get() ; }

    void setTransform(const Matrix2d &trans) ;
    void setSpread(SpreadMethod method) ;

private:
    std::shared_ptr<cairo_pattern_t> pattern_ ;
    Surface surf_ ;
} ;

}

#endif
