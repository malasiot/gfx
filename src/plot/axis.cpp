#include <gfx/axis.hpp>
#include <gfx/canvas.hpp>

#include <fmt/format.h>

#include <iomanip>
using namespace std ;

namespace gfx {



void Axis::computeAxisLayout(double ls, double wsize) {

    labels_.clear() ;
    tic_locations_.clear() ;

    ls_ = ls ;
    unsigned numTics = 2 ;

    double s = wsize - 2*margin_   ;
    double sep ;

    // Compute number of tics for each axis based on the window dimensions

    while (1) {
        sep = (s - ((numTics-1)*ls))/(numTics-1) ;
        if ( sep < label_sep_  ) break ;
        else numTics ++ ;
    }

    // compute scaling factor of displayed labels

    vscale_ = 1.0 ;

    power_ = 0 ;

    if ( !is_log_ ) {
        double v = max(fabs(min_v_), fabs(max_v_)) ;
        while ( v * vscale_ <= 0.1 )  { vscale_ *= 10 ; --power_ ; }
        while ( v * vscale_ > 10.0 ) { vscale_ /= 10 ; ++power_ ; }
    }


    uint tics = numTics ;

    double _max = max_v_, _min = min_v_ ;

    if ( is_log_ )
    {
        if ( _min <= 0 ) throw std::runtime_error("log scale for negative number") ;
        else _min = log10(min_v_) ;

        _max = log10(_max) ;
    }


    tick_locator_->compute(_min, _max, vscale_, numTics, is_log_ ? 1.0 : 0.0, min_label_v_, max_label_v_, tic_locations_) ;


    numTics = tic_locations_.size() ;

    // create labels

    labels_.resize(numTics) ;

    uint i ;

    if ( tick_formatter_ ) {
        for( i=0 ; i<numTics ; i++ ) {
            labels_[i].assign(tick_formatter_->format(tic_locations_[i], i)) ;
        }
    }


    // compute transformation of this axis from data space to window space

    if ( is_reversed_ ) {
        scale_ = (wsize - 2*margin_ )/(max_label_v_ - min_label_v_)*vscale_ ;
        offset_ = -scale_ * max_label_v_ / vscale_  + margin_   ;
    }
    else
    {
       scale_ = double(wsize - 2*margin_)/(max_label_v_ - min_label_v_)*vscale_ ;
        offset_ = -scale_ * min_label_v_ / vscale_  + margin_  ;
   //      scale_ = double(wsize - 2*margin_)/(max_v_ - min_v_) ;
   //      offset_ = -scale_ * min_v_  + margin_  ;
    }

}

Rectangle2d Axis::paintLabel(Canvas &canvas,  const string &text, double x, double y, double ox, double oy)
{
    string mnt, expo ;
    size_t spos = text.find('^') ;
    if ( spos != string::npos )  {
        mnt = text.substr(0, spos) ;
        expo = text.substr(spos+1) ;
    } else {
        mnt = text ;
    }

    Text layout_mnt(mnt) ;
    layout_mnt.setFont(label_font_) ;

    float xoffset = label_font_.size()/8 ; // better to get this from glyph advance

    double lw = layout_mnt.width() ;
    double lh = layout_mnt.height() ;
    double soffset = 0.66*lh ;

    Text layout_expo;

    Font superf(label_font_) ;
    superf.setSize(0.58 * label_font_.size()) ;

    if ( !expo.empty() ) {
        layout_expo.setText(expo);
        layout_expo.setFont(superf) ;

        double ew = layout_expo.width() ;
        double eh = layout_expo.height() ;

        lw += ew + xoffset;
        lh += soffset + eh - lh ;
    }

    double xf = x - ox * lw, yf = y - oy * lh ;

    Rectangle2d layout_rect(xf, yf, lw, lh) ;

    canvas.save() ;
    canvas.setBrush(text_brush_) ;
    canvas.clearPen() ;
    canvas.setFont(label_font_) ;
    canvas.setTextAlign(TextAlignBottom|TextAlignLeft) ;
    canvas.drawText(layout_mnt, layout_rect) ;

    if ( !expo.empty() ) {
        canvas.setFont(superf) ;
        canvas.setTextAlign(TextAlignTop|TextAlignRight) ;
        canvas.drawText(layout_expo, layout_rect) ;
    }

    canvas.restore() ;

    return layout_rect ;
}


void XAxis::computeLayout(double wsize) {
    Text layout("-0.09") ;
    layout.setFont(label_font_) ;
    double maxLabelW = std::round(layout.width()) ;
    computeAxisLayout(maxLabelW, wsize) ;
}

void XAxis::draw(Canvas &canvas, double wsize, double hsize) {

    unsigned s = wsize - 2 * margin_ ;
    unsigned nTics = labels_.size() ;

    double ts = s/(nTics - 1) ;

    canvas.save() ;
    canvas.setFont(label_font_) ;
    canvas.setPen(tics_pen_) ;
    canvas.setTextAlign(TextAlignLeft|TextAlignTop) ;

    canvas.drawLine(0, 0, wsize, 0) ;
    canvas.drawLine(0, -hsize, wsize, -hsize) ;

    double lb = 0 ;

    double ticy = ( tics_placement_ == TicsInside ) ? - tic_size_ :  tic_size_  ;
    double ticy_minor = ( tics_placement_ == TicsInside ) ? - tic_minor_size_  :  tic_minor_size_  ;
    double labely = ticy + (( tics_placement_ == TicsInside ) ? - label_offset_ :  label_offset_ ) ;

    // draw tics

    for(  uint j=0 ; j<nTics ; j++ ) {
        double x1 = tic_locations_[j] * scale_/vscale_ + offset_;

        canvas.drawLine(x1, 0, x1, ticy) ;

        if ( is_log_ && j < nTics - 1) { // minor
            for( int k=2 ; k<10 ; k++ ) {
                double offset = ts*log10((double)k) ;
                canvas.drawLine(x1 + offset, 0, x1 + offset, ticy_minor) ;
            }
        }

        if ( is_log_ )
        {
            Rectangle2d r = paintLabel(canvas, "10^" + labels_[j], x1,labely, 0.5, 0.0);
            lb = std::max(lb, r.height()) ;
        }
        else
        {
            Rectangle2d r = paintLabel(canvas, labels_[j], x1,labely, 0.5, 0.0);
            lb = std::max(lb, r.height()) ;
        }

        if ( draw_grid_ ) {
            canvas.save() ;
            canvas.setPen(grid_pen_) ;
            canvas.drawLine(x1, 0, x1, -hsize) ;
            canvas.restore() ;
        }
    }


    // draw title

    if ( !title_.empty() ) {
        double x1 = margin_ +  s/2 ;
        double y1 = lb + labely + title_offset_ ;

        canvas.setTextAlign(TextAlignTop|TextAlignHCenter) ;
        Rectangle2d boundRect(x1 - title_wrap_/2, y1, title_wrap_, lb) ;

        canvas.save() ;
        canvas.setFont(title_font_) ;
        canvas.setBrush(text_brush_) ;
        canvas.clearPen() ;
        canvas.drawText(title_, boundRect) ;
        canvas.restore() ;
    }

    // draw scaling factor 10^x

    if ( power_ != 0 && dynamic_cast<FixedTickLocator *>(tick_locator_.get()) == nullptr ) {
        double x1 = wsize ;
        double y1 = lb + labely + 5;
        paintLabel(canvas, fmt::format("x10^{}", power_), x1,y1, 1, 0);
    }


    canvas.restore() ;
}

void YAxis::computeLayout(double wsize) {
    Text layout("-0.09") ;
    layout.setFont(label_font_) ;
    double maxLabelH = std::round(layout.height()) ;
    computeAxisLayout(maxLabelH, wsize) ;
}

void YAxis::draw(Canvas &canvas, double wsize, double hsize) {

    unsigned s = hsize - 2 * margin_  ;
    unsigned nTics = labels_.size() ;

    double ts = s/(nTics - 1) ;

    canvas.save() ;
    canvas.setFont(label_font_) ;
    canvas.setPen(tics_pen_) ;
    canvas.setTextAlign(TextAlignLeft|TextAlignTop) ;

    canvas.drawLine(0, 0, 0, -hsize) ;
    canvas.drawLine(wsize, 0, wsize, -hsize) ;

    double lb = 0 ;

    double ticx = ( tics_placement_ == TicsInside ) ?  tic_size_  :  -tic_size_ ;
    double ticx_minor = ( tics_placement_ == TicsInside ) ?  tic_minor_size_  :  -tic_minor_size_  ;
    double labelx = ticx + (( tics_placement_ == TicsInside ) ?  label_offset_ :  -label_offset_) ;

    for(  int j=0 ; j<nTics ; j++ ) {
        double y1 = -j * ts - margin_  ;

        canvas.drawLine(0, y1, ticx, y1) ;

        if ( is_log_ && j < nTics - 1) { // minor
            for( int k=2 ; k<10 ; k++ ) {
                double offset = ts*log10((double)k) ;
                canvas.drawLine(0, y1 + offset, ticx_minor, y1 + offset) ;
            }
        }

        if ( is_log_ )
        {
            Rectangle2d r = paintLabel(canvas, "10^" + labels_[j], labelx, y1, 1.0, 0.5) ;
            lb = std::max(lb, r.width()) ;
        }
        else
        {
            Rectangle2d r = paintLabel(canvas, labels_[j], labelx, y1, 1.0, 0.5);
            lb = std::max(lb, r.width()) ;
        }

        if ( draw_grid_ ) {
            canvas.save() ;
            canvas.setPen(grid_pen_) ;
            canvas.drawLine(0, y1, wsize, y1) ;
            canvas.restore() ;
        }

    }

    if ( !title_.empty() ) {
        double y1 = -margin_  -  s/2 ;
        double x1 =  labelx - lb - title_offset_ ;

        Matrix2d tr ;
        tr.rotate(-M_PI/2.0, Vector2d(x1, y1)) ;

        canvas.save() ;
        canvas.setTransform(tr);

        canvas.setTextAlign(TextAlignHCenter|TextAlignBottom) ;

        const double bs = 2 * lb ; // 2 lines ? (font height)
        Rectangle2d boundRect(x1 - title_wrap_/2, y1 - bs, title_wrap_, bs) ;
        //    canvas.drawRect(boundRect) ;

        canvas.setFont(title_font_) ;
        canvas.setBrush(text_brush_) ;
        canvas.clearPen() ;
        canvas.drawText(title_, boundRect) ;
        canvas.restore() ;

    }

    // draw scaling factor 10^x

    if ( power_ != 0 ) {
        double x1 = 0 ;
        double y1 = -hsize - 5 ;

        Rectangle2d r = paintLabel(canvas, fmt::format("x10^{}", power_), x1,y1, 0, 1.0);
        lb = std::max(lb, r.height()) ;
    }

    canvas.restore() ;
}

double XAxis::transform(double x) {
    if ( is_log_ ) {
        if ( x > 0.0 ) x = log10(x) ;
        else throw runtime_error("log scale needs positive numbers") ;
    }
    return scale_ * x + offset_ ;
}

double YAxis::transform(double y) {
    if ( is_log_ ) {
        if ( y > 0.0 ) y = log10(y) ;
        else throw runtime_error("log scale needs positive numbers") ;
    }
    return -(scale_ * y + offset_) ;
}



}
