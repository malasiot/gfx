#ifndef GFX_CAIRO_BACKEND_HPP
#define GFX_CAIRO_BACKEND_HPP

#include <gfx/bbox.hpp>

#include <cairo/cairo.h>
#include <stack>
#include <memory>

#include <gfx/font.hpp>
#include <gfx/pen.hpp>
#include <gfx/brush.hpp>
#include <gfx/path.hpp>
#include <gfx/surface.hpp>
#include <gfx/text_layout.hpp>

namespace gfx {
namespace detail {

struct State {
     State() ;
     State(const State &other):
            pen_(other.pen_ ? new Pen(*other.pen_) : nullptr),
            brush_(other.brush_ ? other.brush_->clone() : nullptr),
            font_(other.font_),
            trans_(other.trans_),
            text_align_flags_(other.text_align_flags_),
            text_direction_(other.text_direction_),
            text_decoration_(other.text_decoration_)
           {}

     std::unique_ptr<Pen> pen_;
     std::unique_ptr<Brush> brush_ ;

     Font font_ ;
     Matrix2d trans_ ;
     unsigned int text_align_flags_  ;
     TextDirection text_direction_ ;
     TextDecoration text_decoration_ ;
};

class RenderingContext {
public:

    RenderingContext() ;
    ~RenderingContext() ;

    cairo_t *cr_ = nullptr;
    std::stack<State> state_ ;

protected:

    void set_cairo_stroke(const Pen &pen) ;
    void cairo_apply_linear_gradient(const LinearGradientBrush &lg);
    void cairo_apply_radial_gradient(const RadialGradientBrush &rg);
    void cairo_apply_pattern(const PatternBrush &pat);
    void fill_stroke_shape();
    void set_cairo_fill(const Brush *br);
    void line_path(double x0, double y0, double x1, double y1) ;
    void rect_path(double x0, double y9, double w, double h) ;
    void path(const Path &path) ;
    void polyline_path(double *pts, int n, bool) ;
    cairo_t *cr() ;

} ;

cairo_surface_t *cairo_create_image_surface(const Image &im) ;

} // namespace detail
} // namespace gfx ;


#endif
