#include <cairo/cairo.h>

#include <gfx/exception.hpp>

namespace gfx {

const char *BackendException::what() const noexcept {
    return cairo_status_to_string(code_) ;
}

namespace detail {
    void throw_exception_on_cairo_status(cairo_status_t s) {
        if ( s != CAIRO_STATUS_SUCCESS ) {
            throw BackendException(s) ;
        }
    }
}

}
