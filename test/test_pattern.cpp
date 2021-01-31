#include <gfx/canvas.hpp>
#include <gfx/image.hpp>
#include <gfx/vector.hpp>

using namespace gfx ;
using namespace std ;

int main(int argc, char *argv[]) {

    ImageSurface is(512, 512, 92, 92) ;

    Canvas canvas(is) ;

    auto pattern = std::shared_ptr<Surface>(new ImageSurface(64, 64)) ;

    Canvas pc(*pattern) ;

    pc.setBrush(SolidBrush(NamedColor::blue())) ;

    pc.drawCircle(32, 32, 32) ;

    pattern->flush() ;

    static_cast<ImageSurface *>(pattern.get())->getImage().saveToPNG("/tmp/pat.png") ;


    PatternBrush brush(pattern) ;
    brush.setSpread(SpreadMethod::Repeat) ;
    brush.setTransform(Matrix2d::rotation(10*M_PI/180)) ;
    canvas.setBrush(brush) ;
    canvas.drawRect(10, 10, 500, 500) ;

    is.flush() ;

    is.getImage().saveToPNG("/tmp/canvas.png") ;

}
