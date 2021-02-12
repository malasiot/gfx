#include <gfx/surface.hpp>
#include <gfx/impl/canvas.hpp>
#include <cairo-pdf.h>

using namespace std ;

namespace gfx {



Surface::~Surface(){
    flush() ;
}

void Surface::flush() {
    if ( surf_  )
        cairo_surface_flush(surf_.get()) ;
}

void Surface::clip(const Surface &other)
{
    cairo_surface_t *proxy_surf = cairo_surface_create_similar(handle(), CAIRO_CONTENT_COLOR_ALPHA, width_, height_) ;
    cairo_t *cr = cairo_create(proxy_surf) ;

    cairo_set_source_surface(cr, handle(), 0, 0);
    cairo_mask_surface(cr, other.handle(), 0, 0) ;
    cairo_fill(cr) ;
    cairo_surface_destroy(proxy_surf) ;
}

ImageSurface::ImageSurface(int w, int h, double dpix, double dpiy):
    Surface(w, h, dpix, dpiy)  {
    surf_.reset( cairo_image_surface_create(CAIRO_FORMAT_ARGB32, w, h), &cairo_surface_destroy) ;
}


RecordingSurface::RecordingSurface(double width, double height): Surface(width, height, 96, 96) {
    cairo_rectangle_t r{0, 0, width, height} ;
    surf_.reset(cairo_recording_surface_create(CAIRO_CONTENT_COLOR_ALPHA, &r), &cairo_surface_destroy);
}

Image ImageSurface::getImage() const
{
    char *src = (char *)cairo_image_surface_get_data(handle()) ;
    unsigned width = cairo_image_surface_get_width(handle()) ;
    unsigned height = cairo_image_surface_get_height(handle()) ;
    unsigned src_stride = cairo_image_surface_get_stride(handle()) ;
    cairo_format_t src_format = cairo_image_surface_get_format(handle()) ;

    Image im(width, height, ImageFormat::ARGB32) ;

    unsigned dst_stride = im.stride() ;
    char *dst, *p, *q ;
    dst = im.pixels() ;
    uint i, j ;

    if ( src_format == CAIRO_FORMAT_ARGB32 ) {
        for( i=0 ; i<height ; i++, dst += dst_stride, src += src_stride ) {
            for( j=0, p=src, q=dst ; j<width ; j++ ) {
#if __BYTE_ORDER == __LITTLE_ENDIAN
                char a = *p++, r = *p++, g = *p++, b = *p++ ;
#else

                char b = *p++, g = *p++, r = *p++, a = *p++ ;
#endif
                *q++ = a ; *q++ = r ; *q++ = g ; *q++ = b ;
            }
        }
    }

    return im ;
}

PDFSurface::PDFSurface(const string &fileName, int w, int h, double dpi_x, double dpi_y): Surface(w, h, dpi_x, dpi_y) {
    surf_.reset( cairo_pdf_surface_create(fileName.c_str(), w , h),&cairo_surface_destroy ) ;
}

}
