#ifndef GFX_SURFACE_HPP
#define GFX_SURFACE_HPP

#include <gfx/impl/surface.hpp>
#include <gfx/rectangle.hpp>

namespace gfx {

class Surface: public detail::Surface {
protected:
    Surface(double w, double h, double dpix, double dpiy): dpix_(dpix), dpiy_(dpiy),
        width_(w), height_(h) {}
public:

    ~Surface() ;

    virtual void flush() ;
    void clip(const Surface &other) ;

    double width() const { return width_ ; }
    double height() const { return height_ ; }
    double dpiX() const { return dpix_ ; }
    double dpiY() const { return dpiy_ ; }

private:
    double width_, height_, dpix_, dpiy_ ;

};

class ImageSurface: public Surface {
public:
    ImageSurface(int width, int height, double dpi_x = 96, double dpi_y = 96) ;

    Image getImage() const ;
};

class PDFSurface: public Surface {
public:
    PDFSurface(const std::string &path, int width, int height, double dpi_x = 300, double dpi_y = 300);
};

class RecordingSurface: public Surface {
public:
    RecordingSurface(double width, double height) ;
};

class InfiniteRecordingSurface: public Surface {
public:
    InfiniteRecordingSurface(double width, double height) ;

    Rectangle2d getExtents() const ;
};

}

#endif
