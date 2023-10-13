#include <gfx/text_layout.hpp>

#include "impl/text_layout_engine.hpp"
#include "impl/font_manager.hpp"

using namespace std ;

namespace gfx {

TextLayout::TextLayout()
{

}

TextLayout::TextLayout(TextLayout &&t): needs_update_(t.needs_update_), engine_(std::move(t.engine_)) {}

TextLayout::TextLayout(const std::string &text) {
    engine_.reset(new TextLayoutEngine(text)) ;
}

TextLayout::~TextLayout() {}

void TextLayout::setText(const string &text)
{
   engine_.reset(new TextLayoutEngine(text)) ;
}

void TextLayout::setFont(const Font &font)
{
    if ( engine_->getFont() != font ) {
        engine_->setFont(font) ;
        needs_update_ = true ;
    }
}

void TextLayout::setLineSpacing(double ls)
{
    if ( engine_->getLineSpacing() != ls ) {
        engine_->setLineSpacing(ls) ;
        needs_update_ = true ;
    }

}

void TextLayout::setWrapWidth(double tw) {
    if ( engine_->getWrapWidth() != tw ) {
        engine_->setWrapWidth(tw) ;
        needs_update_ = true ;
    }
}

void TextLayout::setTextDirection(TextDirection dir) {
    if ( engine_->getTextDirection() != dir ) {
        engine_->setTextDirection(dir) ;
        needs_update_ = true ;
    }
}

void TextLayout::updateLayout() {
    if ( needs_update_ ) {
        engine_->run() ;
        needs_update_ = false ;
    }
}

const std::vector<GlyphRun> &TextLayout::lines() {
    updateLayout() ;
    return engine_->lines() ;
}

double TextLayout::width() {
    updateLayout() ;
    return engine_->width() ;
}

double TextLayout::height() {
    updateLayout() ;
    return engine_->height() ;
}

Rectangle2d TextLayout::box(const Rectangle2d &box, uint flags)
{
    double tx = std::numeric_limits<double>::max(), ty = 0 ;

    if ( flags & TextAlignVCenter )
        ty = ( box.height() - height())/2;
    else if ( flags & TextAlignBottom )
        ty = box.height() - height()  ;

    for (const GlyphRun &line: lines() ) {
        double ox ;
        if ( flags & TextAlignHCenter )
            ox =  ( box.width() - line.width() )/2.0 ;
        else if ( flags & TextAlignRight )
            ox =  box.width() - line.width();

        tx = std::min(ox, tx) ;
    }

    return Rectangle2d(box.x() + tx, box.y() + ty, width(), height()) ;
}

}
