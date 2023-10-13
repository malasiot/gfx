#include <gfx/canvas.hpp>
#include <gfx/image.hpp>
#include <gfx/vector.hpp>

#include <gfx/bbox.hpp>

#include <gfx/plot/axis.hpp>
#include <gfx/plot/plot.hpp>

#include <cmath>

using namespace gfx ;
using namespace std ;

class CustomMarker: public AdaptiveCircleMarker {
public:
    CustomMarker(uint ns) {


    }

    Color getOutlineColor(uint idx) const override{
        return Color(NamedColor::red(), 0.5) ;

    }
    Color getFillColor(uint idx) const override {
        if ( idx > 0)
            return Color(NamedColor::green(), 0.5) ;
        else
            return Color(NamedColor::red(), 0.5) ;

    }

    double getRadius(uint i) const override {
        return  i * 2 + 1 ;
    }

};

int main(int argc, char *argv[]) {

    ImageSurface is(1024, 1024) ;
    Canvas canvas(is) ;

    canvas.clearPen() ;
    canvas.fill(NamedColor::white()) ;

    Font f("Arial", 32) ;
    f.setStyle(FontStyle::Italic) ;
    canvas.setFont(f) ;

    Matrix2d tr ;
    tr.translate(0, 500) ;

 //   canvas.setTransform(tr);

    Plot plot ;

    vector<double> x1 = { 0.0, 0.1, 0.6 } ;
    vector<double> y1 = { 1.0, 3.1, -0.4 } ;

    vector<double> x2 = { 0.0, 0.4, 0.9 } ;
    vector<double> y2 = { -1.0, 1.1, -0.4 } ;

    vector<double> e = {0.3, 0.3, 0.2} ;

    vector<double> rast;

    uint nrows = 240, ncols = 240 ;

    for( uint i=0 ; i<nrows ; i++ ) {
        float y = -3.0 + i * 0.025 ;
        for( uint j=0 ; j<ncols ; j++ ) {
            float x = -3.0 + j * 0.025 ;

            float z1 = exp(-x*x - y*y) ;
            float z2 = exp(-(x-1)*(x-1) - (y-1)*(y-1)) ;
            float z = (z1 - z2) * (z1- z2) ;
            rast.push_back(z) ;
        }
    }
    auto &graph0 = plot.raster(rast, nrows, ncols, {-3, -3, 3, 3}) ;
    graph0.setColorMap(new HotColorMap()).setInterpolation(RasterInterpolation::Flat) ;

    plot.addColorRamp(new HotColorMap(), 0.0, 1.0) ;

    plot.setAspect(1.0) ;
  //  auto &graph1 = plot.lines(x1, y1, "rx--") ;
 //   graph1.setTitle("line 1") ;
 //   graph1.pen().setLineWidth(2) ;
  //  graph1.setMarker(new CustomMarker(2)) ;

 //   graph1.setBrush(new SolidBrush(Color(0.5, 0.2, 0.1, 0.5)));

//    auto &graph2 = plot.lines(x2, y2, "g+:") ;
 //    graph2.setTitle("line 2") ;
   // graph2.pen().setColor(NamedColor::green()) ;


    plot.xAxis().setGrid(true).setTitle("X").setMargin(0) ;
    plot.yAxis().setGrid(true).setTitle("Y").setMargin(0).setReversed(false) ;
   // plot.yAxis().setReversed(true) ;
    plot.setTitle("Text plot") ;
    plot.legend().setPlacement(Legend::None) ;


    plot.draw(canvas, 500, 500) ;

    is.flush() ;
    is.getImage().saveToPNG("/tmp/oo.png") ;
}
