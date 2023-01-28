#ifndef GFX_CANVAS_HPP
#define GFX_CANVAS_HPP

#include <memory>

#include <string>
#include <gfx/pen.hpp>
#include <gfx/brush.hpp>
#include <gfx/font.hpp>
#include <gfx/xform.hpp>
#include <gfx/path.hpp>
#include <gfx/font.hpp>
#include <gfx/surface.hpp>

#include <gfx/image.hpp>
#include <gfx/rectangle.hpp>
#include <gfx/glyph.hpp>
#include <gfx/text_layout.hpp>
#include <gfx/svg_document.hpp>

#include <gfx/impl/canvas.hpp>

namespace gfx {

enum BlendMode {
    CLEAR, SRC, SRC_OVER, SRC_IN, SRC_OUT, SRC_ATOP, DST, DST_OVER, DST_IN, DST_OUT, DST_ATOP, XOR, ADD,
    SATURATE, MULTIPLY, SCREEN, OVERLAY, DARKEN, LIGHTEN, DODGE, BURN, HARD_LIGHT, SOFT_LIGHT, DIFFERENCE,
    EXCLUSION, HSL_HUE, HSL_COLOR, HSL_LUMINOSITY
};

class StyledText ;

class Canvas: public detail::CanvasImpl {
protected:
    Canvas(Canvas &&op) = delete ;
    Canvas& operator=(Canvas&& op) = delete ;
public:
   ~Canvas() ;
    Canvas(Surface &surface) ;
    Canvas(cairo_t *cr, float w, float h, float dpi) ;

    //const Surface &surface() const { return surface_ ; }

    void save() ;
    void restore() ;

    void setTransform(const Matrix2d &tr) ;

    void setPen(const Pen &pen) ;
    void clearPen() ;

    void setBrush(const Brush &brush) ;
    void clearBrush() ;

    void setFont(const Font &font) ;
    void setBlendMode(BlendMode mode) ;
    void setTextAlign(int flags) ;
    void setTextDirection(TextDirection dir) ;
    void setTextDecoration(TextDecoration dec) ;

    void setAntialias(bool antiAlias = true) ;

    void setClipRect(double x0, double y0, double w, double h) ;
    void setClipRect(const Rectangle2d &r) ;
    void setClipPath(const Path &p, FillRule frule= FillRule::EvenOdd) ;

    void drawLine(double x0, double y0, double x1, double y1) ;
    void drawLine(const Point2d &p1, const Point2d &p2) ;
    void drawRect(double x0, double y0, double w, double h) ;
    void drawRect(const Rectangle2d &r) ;

    void drawPath(const Path &path) ;

    void drawPolyline(double *pts, int nPts) ;
    void drawPolygon(double *pts, int nPts) ;
    void drawCircle(double cx, double cy, double r) ;
    void drawCircle(const Point2d &center, double r) ;
    void drawEllipse(double xp, double yp, double ax, double ay) ;

    void drawText(const std::string &textStr, double x0, double y0) ;
    void drawText(const std::string &textStr, double x0, double y0, double width, double height) ;
    void drawText(const std::string &textStr, const Point2d &p) ;
    void drawText(const std::string &textStr, const Rectangle2d &r) ;

    void drawText(Text &text, double x0, double y0) ;
    void drawText(Text &text, double x0, double y0, double width, double height) ;
    void drawText(Text &text, const Point2d &p) ;
    void drawText(Text &text, const Rectangle2d &r) ;

    void drawGlyph(const Glyph &g, const Point2d &p) ;
    void drawGlyphs(const std::vector<Glyph> &glyphs, const std::vector<Point2d> &positions) ;

    void drawImage(const Image &im,  double opacity) ;

    void drawSVG(const SVGDocument &doc, double target_width, double target_height) ;

    void fill(const Color &color) ;

    float surfaceDpiX() const { return dpi_x_ ; }
    float surfaceDpiY() const { return dpi_y_ ; }
    float surfaceWidth() const { return width_ ; }
    float surfaceHeight() const { return height_ ; }

protected:
    float width_, height_, dpi_x_, dpi_y_ ;
} ;

}

#endif
