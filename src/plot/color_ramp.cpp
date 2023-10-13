#include <gfx/plot/color_ramp.hpp>

namespace gfx {


Rectangle2d ColorRamp::layout(double h) {

    double height = h * fraction_ ;

    axis_.computeLayout(height) ;

    double ox = axis_.size() ;

    float width = pad_ + width_ + ox ;

    return Rectangle2d(0, 0, width, height) ;
}

void ColorRamp::paint(Canvas &c, double w, double h) {

    float height = h * fraction_ ;

    layout(h) ;

    Image im = renderBar(width_, height) ;

    c.save() ;
    c.setTransform({1.0, 0.0, 0.0, -1.0, w + pad_, -(h/2-height/2)});
    c.drawImage(im, 1) ;
    c.restore() ;

    c.save() ;
    c.setPen(Pen());
    c.drawRect(w + pad_, -(h/2-height/2), width_, -height );
    c.restore() ;

    c.save() ;
    c.setTransform({1.0, 0.0, 0.0, 1.0, pad_ + width_ + w, -(h/2-height/2)}) ;
    axis_.draw(c, w, height) ;
    c.restore() ;

}

double ColorRamp::measureWidth(double h) {
    return axis_.measureWidth(h) + width_ + pad_ ;
}

Image ColorRamp::renderBar(size_t width, size_t height) {
    Image im(width, height, ImageFormat::ARGB32) ;

    for( int i=0 ; i<height; i++ ) {
        float v = ((float)i)/(height-1) ;
        auto clr = cmap_->map(v) ;

        for( int j=0 ; j<width ; j++ ) {
            char *pixel = im.pixels() + im.stride() * i + 4 * j ;
            *pixel++ = 255 ;
            *pixel++ = char(clr.r() * 255) ;
            *pixel++ = char(clr.g() * 255) ;
            *pixel++ = char(clr.b() * 255) ;
        }
    }

    return im ;
}
}
