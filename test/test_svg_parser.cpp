#include <gfx/canvas.hpp>

#include <fstream>
#include <iostream>

using namespace gfx ;
using namespace std ;

int main(int argc, char *argv[]) {


    SVGDocument doc ;

 //   ifstream strm("/home/malasiot/Downloads/adobe.svg") ;
    ifstream strm("/home/malasiot/Downloads/symbols/place_of_worship/s_church.svg") ;
    try {
        doc.readStream(strm) ;


        ImageSurface is(580, 580, 96) ;
        Canvas canvas(is) ;

        canvas.fill(Color(NamedColor::black(), 0.0));
     //   canvas.drawRect(0, 0, 1024, 512) ;

        canvas.drawSVG(doc, 580, 580) ;

        is.flush() ;
        is.getImage().saveToPNG("/tmp/oo.png") ;


    }
    catch ( SVGLoadException &e ) {

        cout << e.what() << endl ;
    }

}
