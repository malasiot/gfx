#pragma once

#include <gfx/surface.hpp>

#include <QPainter>

namespace gfx {

class QtSurface: public ImageSurface {
public:
    QtSurface(QPainter *p) ;

    void flush() override ;

private:

    QPainter *painter_ ;
};


}
