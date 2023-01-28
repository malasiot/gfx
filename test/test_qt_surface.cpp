#include <gfx/canvas.hpp>
#include <gfx/image.hpp>
#include <gfx/vector.hpp>
#include <gfx/plot.hpp>

#include <gtkmm.h>

using namespace gfx;
using namespace std ;


void makePlot(Plot &plot) {
    vector<double> x1 = { 0.0, 0.1, 0.6 } ;
    vector<double> y1 = { 1.0, 3.1, -0.4 } ;

    vector<double> x2 = { 0.0, 0.4e3, 0.9e6 } ;
    vector<double> y2 = { -1.0, 1.1, -0.4 } ;

    vector<double> e = {0.3, 0.3, 0.2} ;

    auto &graph1 = plot.lines(x1, y1, "r--") ;
    graph1.setTitle("line 1") ;
    graph1.pen().setLineWidth(2) ;
    graph1.setErrors(e) ;
    graph1.setBrush(new SolidBrush(Color(0.5, 0.2, 0.1, 0.5)));

    auto &graph2 = plot.lines(x2, y2, "g+:") ;
     graph2.setTitle("line 2") ;
   // graph2.pen().setColor(NamedColor::green()) ;


    plot.xAxis().setGrid(true).setTitle("X") ;
    plot.yAxis().setGrid(true).setTitle("Y") ;
    plot.setTitle("Text plot") ;
    plot.legend().setPlacement(Legend::BottomRight) ;
}

void drawShape(Canvas &canvas, const Rectangle2d &rect) {
    canvas.clearPen() ;
    canvas.setBrush(SolidBrush(NamedColor::blue())) ;

    canvas.drawRect(rect) ;

    canvas.setPen(Pen(NamedColor::red(), 2));
    canvas.drawLine(rect.topLeft(), rect.bottomRight()) ;
    canvas.drawLine(rect.bottomLeft(), rect.topRight()) ;

    canvas.clearPen() ;
    canvas.setBrush(SolidBrush(NamedColor::white())) ;

    Path p ;
    p.addText("Hello", rect.width()/2, rect.height()/2, Font("Arial,serif", 12)) ;
    canvas.drawPath(p) ;
}


class PlotArea : public Gtk::DrawingArea
{
public:
    PlotArea(std::shared_ptr<Plot> plot): plot_(plot) {

    }
    virtual ~PlotArea(){}

protected:
    bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override {
        Gtk::Allocation allocation = get_allocation();
        const int width = allocation.get_width();
        const int height = allocation.get_height();

        Canvas c(cr->cobj(), width, height, 96) ;
        c.setAntialias();

        Matrix2d tr ;
        tr.translate(40, height-40) ;
        c.setTransform(tr);

        plot_->draw(c, width-80, height-80) ;

        /*

        Rectangle2d rect(0, 0, width, height) ;
        drawShape(c, rect) ;
*/
        return true ;


    }

    std::shared_ptr<Plot> plot_ ;
};



int main(int argc, char *argv[]) {
    auto app = Gtk::Application::create("org.gtkmm.examples.base");

    Gtk::ApplicationWindow win;

     win.set_title("Cairomm Clock");
     win.set_default_geometry(1000, 1000);

     auto plot = std::make_shared<Plot>() ;
     makePlot(*plot) ;

     PlotArea c(plot);

     win.add(c);
     c.show();

     return app->run(win);

}
