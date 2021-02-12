#include <gfx/canvas.hpp>
#include <gfx/image.hpp>
#include <gfx/vector.hpp>

using namespace gfx ;
using namespace std ;

int main(int argc, char *argv[]) {

    ImageSurface is(512, 512, 92, 92) ;

    Canvas canvas(is) ;

    ImageSurface pattern(64, 64) ;

    Canvas pc(pattern) ;

    pc.fill({1, 1, 1, 0});
    pc.setBrush(SolidBrush(NamedColor::blue())) ;

    pc.drawCircle(32, 32, 32) ;

    pattern.flush() ;

    pattern.getImage().saveToPNG("/tmp/pat.png") ;

    PatternBrush brush(pattern) ;
    brush.setSpread(SpreadMethod::Repeat) ;
    brush.setTransform(Matrix2d::rotation(10*M_PI/180)) ;

    canvas.fill({1, 1, 1, 0});

    canvas.setTransform(Matrix2d::scaling(0.2, 0.2)) ;
    canvas.setBrush(brush) ;
    canvas.drawRect(10, 10, 500, 500) ;



    is.flush() ;

    is.getImage().saveToPNG("/tmp/canvas.png") ;

}
