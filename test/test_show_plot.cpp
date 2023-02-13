#include <gfx/canvas.hpp>
#include <gfx/image.hpp>
#include <gfx/vector.hpp>
#include <gfx/plot/plot.hpp>
#include <gfx/plot/bar_chart.hpp>

#include <iostream>
#include <thread>

using namespace gfx;
using namespace std ;


void makeLinePlot(Plot &plot) {
    vector<double> x1 = { -0., 1, 1.5 } ;
    vector<double> y1 = { 1.0, 3.1, -0.4 } ;

    vector<double> x2 = { 0.0, 0.4, 0.9 } ;
    vector<double> y2 = { -1.0, 1.1, -0.4 } ;

    auto &graph1 = plot.lines(x1, y1, "r--") ;
    graph1.setTitle("line 1") ;

    graph1.pen().setLineWidth(2) ;
 //   graph1.setErrors(e) ;
//    graph1.setBrush(new SolidBrush(Color(0.5, 0.2, 0.1, 0.5)));

    auto &graph2 = plot.lines(x2, y2, "g+:") ;
     graph2.setTitle("line 2") ;
      graph2.pen().setLineWidth(1) ;
   // graph2.pen().setColor(NamedColor::green()) ;


   plot.xAxis().setGrid(true).setTitle("X") ;
    plot.yAxis().setGrid(true).setTitle("Y") ;
    plot.setTitle("Text plot") ;
    plot.legend().setPlacement(Legend::TopRight) ;
}

void makeBarChart(Plot &plot) {

    vector<double> v1{1, 2, 0.5} ;
    plot.bars(v1, 0.2)
            .setPen(new Pen(NamedColor::brown()))
            .setTitle("Categories") ;
    plot.bars({1.2, 2.5, 0.8}, v1, 0.2, 0, NamedColor::aqua()).setTitle("Images") ;
    plot.errorbars({0, 1, 2}, v1, {{0.1, 0.05, 0.2}}) ;

    plot.xAxis().setTicks({0, 1, 2}, {"A", "B", "C"});
    plot.yAxis().setMargin(0).setGrid(true) ;
}

int main(int argc, char *argv[]) {

  //  auto plot1 = std::make_shared<Plot>() ;
  //  makePlot(*plot1, true) ;


  auto plot2 = std::make_shared<Plot>() ;
  makeBarChart(*plot2) ;

  //  std::thread t1(&GUI::, GUI::instance(), plot, "test1") ;
 //   std::thread t2(&GUI::init, GUI::instance()) ;

    plot2->show("test2", true) ;

}
