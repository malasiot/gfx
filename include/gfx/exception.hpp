#ifndef GFX_EXCEPTION_HPP
#define GFX_EXCEPTION_HPP

#include <stdexcept>

#include <cairo/cairo.h>

namespace gfx {

class BackendException: public std::exception {
public:
    BackendException(cairo_status_t status_code): code_(status_code) {}

    const char *what() const noexcept override;

private:
    cairo_status_t code_ ;
};

namespace detail {
    void throw_exception_on_cairo_status(cairo_status_t s) ;
}

}

#endif
