#include <gfx/canvas.hpp>
#include <gfx/image.hpp>
#include <gfx/vector.hpp>
#include <gfx/plot.hpp>
#include <iostream>
#include <thread>

using namespace gfx;
using namespace std ;


void makePlot(Plot &plot, bool f) {
    vector<double> x1 = { 0.0, 0.1, 0.6 } ;
    vector<double> y1 = { 1.0, 3.1, -0.4 } ;

    vector<double> x2 = { 0.0, 0.4e3, 0.9e6 } ;
    vector<double> y2 = { -1.0, 1.1, -0.4 } ;

    vector<double> e = {0.3, 0.3, 0.2} ;
if ( f ) {
    auto &graph1 = plot.lines(x1, y1, "r--") ;
    graph1.setTitle("line 1") ;
    graph1.pen().setLineWidth(2) ;
    graph1.setErrors(e) ;
    graph1.setBrush(new SolidBrush(Color(0.5, 0.2, 0.1, 0.5)));
} else {
    auto &graph2 = plot.lines(x2, y2, "g+:") ;
     graph2.setTitle("line 2") ;
   // graph2.pen().setColor(NamedColor::green()) ;
}

    plot.xAxis().setGrid(true).setTitle("X") ;
    plot.yAxis().setGrid(true).setTitle("Y") ;
    plot.setTitle("Text plot") ;
    plot.legend().setPlacement(Legend::BottomRight) ;
}



int main(int argc, char *argv[]) {

    auto plot1 = std::make_shared<Plot>() ;
    makePlot(*plot1, true) ;

    auto plot2 = std::make_shared<Plot>() ;
    makePlot(*plot2, false) ;

  //  std::thread t1(&GUI::, GUI::instance(), plot, "test1") ;
 //   std::thread t2(&GUI::init, GUI::instance()) ;

    plot1->show("test1") ;
 //    GUI::showPlot(plot, "test2") ;
  std::this_thread::sleep_for(std::chrono::milliseconds(2000));

     plot2->show() ;

      std::this_thread::sleep_for(std::chrono::milliseconds(2000));
}
