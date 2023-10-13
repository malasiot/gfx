#include <gfx/svg_document.hpp>
#include <gfx/canvas.hpp>

#include "svg_parser.hpp"
#include "svg_render_context.hpp"

#include "string_utils.hpp"
#include "base64.hpp"

#include <sstream>

using namespace std ;
using namespace svg ;


namespace gfx {

SVGLoadException::SVGLoadException(const string &error_msg, size_t line, size_t col):
    msg_(error_msg), line_(line), col_(col) {}

std::string SVGLoadException::what() {
    ostringstream strm ;
    strm << "XML parse error: " << msg_ << " line: " << line_ << ", column: " << col_ ;
    return strm.str() ;
}

void SVGDocument::readStream(std::istream &strm) {
    SVGParser parser(*this) ;
    parser.parseStream(strm) ;
}

void SVGDocument::getDimensions(double &w, double &h, double fw, double fh) const
{
    svg::Context ctx(fw, fh) ;
    ctx.setTargetSizeHint(fw, fh) ;

    auto root = std::dynamic_pointer_cast<svg::SVGElement>(getDOM()) ;

    w = ctx.toPixels(root->width(), LengthDirection::Horizontal) ;
    h = ctx.toPixels(root->height(), LengthDirection::Vertical) ;
}

void SVGDocument::setLocalResourceFolder(const string &folder) {
    resource_folder_ = folder ;
}

void SVGDocument::registerNamedElement(const string &id, svg::Element *e) {
    elements_.insert({"#" + id, e}) ;
}


svg::Element *SVGDocument::resolve(const std::string &uri) const {
    if ( uri.empty() ) return nullptr ;
    auto it = elements_.find(uri) ;
    if ( it != elements_.end() ) {
        svg::Element *p = it->second ;
        return p ;
    }
    return nullptr ;
}

Image SVGDocument::loadImageResource(const string &uri, svg::Element *container) {
    auto it = cached_images_.find(container) ;
    if ( it != cached_images_.end() ) return it->second ;

    if ( ends_with(uri, "data:image/png;base64" ) ) {
        string png_data = base64_decode(uri.substr(22)) ;
        auto image =  Image::loadPNGBuffer(png_data) ;
        cached_images_[container] = image ;
        return image ;
    } else {
        if ( ends_with(uri, ".png") ) {
            Image im = Image::loadPNG(resource_folder_ + '/' + uri) ;
            return im ;
        }
        else if ( ends_with(uri, ".svg") ) {
            return Image() ;
        }

    }

    return Image() ;
}

void Canvas::drawSVG(const SVGDocument &doc, double w, double h)
{
    svg::RenderingContext ctx(*this, w, h) ;
    ctx.setTargetSizeHint(w, h) ;

    auto root = std::dynamic_pointer_cast<svg::SVGElement>(doc.getDOM()) ;

    // if the document has specified width and height we need to scale to
    // requested dimensions

    float sw = ctx.toPixels(root->width(), LengthDirection::Horizontal) ;
    float sh = ctx.toPixels(root->height(), LengthDirection::Vertical) ;

    float scalex = w/sw ;
    float scaley = h/sh ;

    save() ;
    setTransform(Matrix2d::scaling(scalex, scaley)) ;
    ctx.render(*root) ;
    restore();
}


}
