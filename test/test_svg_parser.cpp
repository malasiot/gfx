#include <gfx/canvas.hpp>

#include <fstream>
#include <iostream>

using namespace gfx ;
using namespace std ;

int main(int argc, char *argv[]) {


    SVGDocument doc ;

 //   ifstream strm("/home/malasiot/Downloads/adobe.svg") ;
    ifstream strm("/home/malasiot/Downloads/ele_res/p_wood-mixed.svg") ;
    try {
        doc.readStream(strm) ;

        ImageSurface ss(128, 128) ;
        Canvas cc(ss) ;
        ImageSurface is(128, 128, 96) ;
        Canvas canvas(is) ;

        canvas.setBrush(SolidBrush(Color(NamedColor::white(), 0))) ;
     //   canvas.drawRect(0, 0, 1024, 512) ;

        canvas.drawSVG(doc, 128, 128) ;

        is.flush() ;
        is.getImage().saveToPNG("/tmp/oo.png") ;

        ImageSurface icanvas(256, 256) ;
        Canvas c2(icanvas) ;

        c2.setTransform(Matrix2d::translation(50, 50)) ;
        c2.drawImage(is.getImage(), 1.0) ;

        icanvas.flush() ;
        icanvas.getImage().saveToPNG("/tmp/canvas.png");


    }
    catch ( SVGLoadException &e ) {

        cout << e.what() << endl ;
    }

}
