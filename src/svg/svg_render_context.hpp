#ifndef GFX_SVG_RENDER_CONTEXT_HPP
#define GFX_SVG_RENDER_CONTEXT_HPP

#include <gfx/canvas.hpp>
#include "svg_dom.hpp"

namespace svg {

enum class RenderingMode { Display, BoundingBox, Cliping } ;

struct Context {
    Context(float fw, float fh) {
        view_boxes_.push_back({0, 0, fw, fh}) ;
        font_sizes_.push_back(12) ;
    }

    void setDpi(float dpix, float dpiy) {
        dpi_x_ = dpix ; dpi_y_ = dpiy ;
    }

    void setTargetSizeHint(double w, double h) {
        doc_width_hint_ = w ;
        doc_height_hint_ = h ;
    }

    float toPixels(const Length &l, LengthDirection dir, bool scale_to_viewport = true) ;

    std::deque<ViewBox> view_boxes_ ;
    std::deque<double> font_sizes_ ;
    float doc_width_hint_, doc_height_hint_ ;
    float dpi_x_ = 92, dpi_y_ = 92 ;
    gfx::Rectangle2d obbox_  ;
} ;

class RenderingContext: public Context {

public:

    RenderingContext(gfx::Canvas &canvas, float fw, float fh, RenderingMode m = RenderingMode::Display): Context(fw, fh), canvas_(canvas), rendering_mode_(m) {
        const gfx::Surface &s = canvas.surface() ;
        dpi_x_ = s.dpiX() ;
        dpi_y_ = s.dpiY() ;
    }
    void pushState(const Style &) ;
    void popState() ;
    void pushTransform(const Matrix2d &) ;
    void popTransform() ;

    void preRenderShape(Element &e, const Style &s, const Matrix2d &tr, const gfx::Rectangle2d &rect) ;
    void setPaint(Element &e) ;
    void postRenderShape() ;

    void applyClipPath(ClipPathElement *e) ;

    void extentBoundingBox(double x1, double x2, double y1, double y2) ;

    void populateRefs(const svg::ElementPtr &root)  ;
    ElementPtr lookupRef(const std::string &name) ;

    void setFilePath(const std::string &path) {
        file_path_ = path ;
    }

    void render(SVGElement &)  ;
    void render(CircleElement &) ;
    void render(LineElement &) ;
    void render(PolygonElement &) ;
    void render(PolylineElement &) ;
    void render(PathElement &) ;
    void render(RectElement &) ;
    void render(EllipseElement &) ;
    void render(GroupElement &) ;
    void render(SymbolElement &, double pw, double ph) ;
    void render(UseElement &) ;
    void render(ImageElement &) ;
    void render(TextElement &) ;
    void render(TSpanElement &) ;
    void render(TRefElement &) ;


    void render(Element *e) ;
    void renderChildren(const Element &e) ;

    void clip(Element &e, const Style &st);
    void clip(Element *e) ;
    void clipChildren(const Element &e);

    void addClipPath(const Path &p) ;

    void setShapeAntialias(ShapeQuality aa);
    void setLinearGradientBrush(LinearGradientElement &e, float a) ;
    void setRadialGradientBrush(RadialGradientElement &e, float a) ;
    void setPatternBrush(PatternElement &e, float a) ;
    gfx::Font makeFont(const Style &st);

    void setOverflow(const Style &st, const gfx::Rectangle2d &r);
protected:

    gfx::Canvas &canvas_ ;
    std::deque<Style> states_ ;
    std::deque<Matrix2d> transforms_ ;



    RenderingMode rendering_mode_ ;
    std::map<std::string, ElementPtr> refs_ ;
    Matrix2d view2dev_ ;

    double cursor_x_, cursor_y_ ;
    std::string file_path_ ;


    Path clip_path_ ;
};


}































#endif
