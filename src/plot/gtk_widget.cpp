#include "gtk_widget.hpp"
#include "safe_queue.hpp"

#include <thread>
#include <iostream>

using namespace std ;

namespace gfx {

void PlotArea::setPlot(const std::shared_ptr<Plot> &plot) {
    plot_ = plot ;
    queue_draw();
}

bool PlotArea::on_draw(const Cairo::RefPtr<Cairo::Context> &cr) {
    if ( !plot_ ) return false ;

    Gtk::Allocation allocation = get_allocation();
    const int width = allocation.get_width();
    const int height = allocation.get_height();

    InfiniteRecordingSurface rs(width, height) ;
    Canvas cc(rs) ;
    plot_->draw(cc, width, height) ;
    Rectangle2d r = rs.getExtents() ;
    double mx = r.width() - width ;
    double my = r.height() - height ;

    Canvas c(cr->cobj(), width, height, 96) ;
    c.setAntialias();

    Matrix2d tr ;
    tr.translate(-r.x()+margin_, height -( r.y() + r.height()) - margin_) ;
    c.setTransform(tr);

    plot_->draw(c, width-mx-2*margin_, height-my- 2*margin_) ;

    return true ;
}


using q_fn = std::function<void ()>;

class IdleQueue
{
public:
    IdleQueue()
        : m_safe_queue()
        , m_dispatcher()
    {
        m_dispatcher.connect(sigc::bind(&IdleQueue::processJobs, this));
    }

    void dispatch(q_fn func)
    {
        m_safe_queue.enqueue(std::move(func));
        m_dispatcher.emit();
    }
private:
    SafeQueue<q_fn> m_safe_queue;
    Glib::Dispatcher m_dispatcher;

    // This function will always take place on the main thread.
    void processJobs()
    {
        for (;;) // Eagerly process all jobs on queue.
        {
            const std::optional<q_fn> &job_opt = m_safe_queue.try_dequeue();
            if (!job_opt.has_value()) return;

            try
            {
                job_opt.value()();
            }
            catch (std::exception &e)
            {
                // TODO: Figure out what to do with errors.
                std::cout << "Got exception while processing job: " << e.what();
            }
        }
    }
};

class GUI {
public:
    GUI() {

    }

    ~GUI() {
        if ( app_ ) app_->quit() ;
        if ( thread_ ) thread_->join() ;
    }
    void init() {
        if ( !app_ ) {
        app_ = Gtk::Application::create("org.gtkmm.examples.base", Gio::ApplicationFlags::APPLICATION_NON_UNIQUE);
      //  app_->signal_activate().connect(sigc::bind(sigc::mem_fun(instance_.get(), &GUI::on_activate), plot, name)) ;
      app_->register_application() ;

        window_.reset(new Gtk::Window) ;

        app_->add_window(*window_) ;
        window_->set_default_geometry(1000, 1000);

        plot_area_.reset(new PlotArea(nullptr)) ;

        window_->add(*plot_area_);
        window_->show_all() ;

        app_->run() ;
      }


    }

    void create() {
        if ( !thread_ )
            thread_.reset(new std::thread(&GUI::init, this)) ;
    }

    static GUI *instance() { return instance_.get() ; }


     void on_notification(const std::shared_ptr<Plot> &plot, const std::string &name) {

         window_->set_title(name) ;
         plot_area_->setPlot(plot) ;


     }

     static void showPlot(const std::shared_ptr<Plot> &plot, const std::string &name) {

         instance_->create() ;
         instance_->queue_.dispatch(std::bind(&GUI::on_notification, instance_.get(), plot, name));

      }


private:
     std::map<std::string, Gtk::ApplicationWindow> windows_ ;
     IdleQueue queue_;
     std::unique_ptr<PlotArea> plot_area_ ;
     std::unique_ptr<Gtk::Window> window_ ;
     Glib::RefPtr<Gtk::Application> app_ ;
     std::unique_ptr<std::thread> thread_ ;
     static std::unique_ptr<GUI> instance_ ;

};

std::unique_ptr<GUI> GUI::instance_( new GUI() );


void Plot::show(const std::string &title) {
    auto ptr = shared_from_this();
    GUI::showPlot(ptr, title) ;
}


}
