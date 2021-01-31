#ifndef GFX_DETAIL_SURFACE_HPP
#define GFX_DETAIL_SURFACE_HPP

#include <cairo/cairo.h>

#include <gfx/image.hpp>

namespace gfx {
namespace detail {

class Surface {
public:

    cairo_surface_t *surf_ = nullptr ;
    double dpix_, dpiy_ ;
};

}
}
#endif
