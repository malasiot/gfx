#ifndef GFX_DETAIL_SURFACE_HPP
#define GFX_DETAIL_SURFACE_HPP

#include <cairo/cairo.h>

#include <gfx/image.hpp>

namespace gfx {
namespace detail {

class Surface {
public:
    cairo_surface_t *handle() const { return surf_.get() ; }

protected:

    Surface(): surf_(nullptr, &cairo_surface_destroy) {}

    using surface_handle = std::shared_ptr<cairo_surface_t> ;

    surface_handle surf_  ;
    double dpix_, dpiy_ ;
};

}
}
#endif
