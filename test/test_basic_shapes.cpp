#include <gfx/canvas.hpp>
#include <gfx/image.hpp>
#include <gfx/vector.hpp>

using namespace gfx ;
using namespace std ;

void drawShape(Canvas &canvas, const Rectangle2d &rect) {

    canvas.drawRect(rect) ;
    canvas.drawLine(rect.topLeft(), rect.bottomRight()) ;
    canvas.drawLine(rect.bottomLeft(), rect.topRight()) ;

    Path p ;
    p.addText("Hello", rect.x(), rect.y(), Font("Arial,serif", 12)) ;
    canvas.drawPath(p) ;
}

int main(int argc, char *argv[]) {
/*
    ImageSurface ms(400, 400) ;
    Canvas mask(ms) ;

    mask.fill(Color(0, 0, 0, 0));
    mask.setBrush(SolidBrush(NamedColor::white())) ;
    mask.drawCircle(100, 100, 250) ;

    ms.flush() ;
    ms.getImage().saveToPNG("/tmp/mask.png") ;
*/
    ImageSurface is(1024, 512) ;
    Canvas canvas(is) ;


 //   canvas.fill(NamedColor::white()) ;

    canvas.clearPen() ;
    canvas.setBrush(SolidBrush(NamedColor::blue())) ;

    canvas.save() ;

    Rectangle2d rect(0, 0, 100, 100) ;

 //   canvas.setTransform(Matrix2d::translation({50, 50})) ;

    canvas.setBrush(SolidBrush({0.1, 0.3, 0.5})) ;

    Path clipPath ;

    clipPath.addRect(0, 0, 1024, 512) ;
    clipPath.addArc(100, 100, 50, 50, 0, 360) ;
    clipPath.closePath() ;

    // https://www.cairographics.org/cookbook/outside_clipping/
    canvas.setClipPath(clipPath, FillRule::EvenOdd) ;

    canvas.setBlendMode(BlendMode::XOR) ;
    double angle = 0 ;
    for( double x = 0 ; x < 1024 ; x += 200 )
        for( double y = 0 ; y<512 ; y += 200 ) {

            angle += 10 * M_PI/180 ;

            Matrix2d tr ;
            tr.rotate(angle, rect.center(), 2 ) ;
            tr.translate({x, y}) ;

            canvas.save() ;

            Path clipPath ;

            clipPath.addRect(0, 0, 30, 30) ;

            canvas.setTransform(tr) ;
        //    canvas.setClipPath(clipPath) ;

            drawShape(canvas, rect) ;

            canvas.restore() ;


        }


    canvas.restore() ;



    Image xim = Image::loadPNG("/home/malasiot/Downloads/image.png");

    canvas.drawImage(xim, 0.5) ;


    is.flush() ;

  //  is.clip(ms) ;
    is.getImage().saveToPNG("/tmp/oo.png") ;

}
