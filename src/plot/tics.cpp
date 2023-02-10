#include <gfx/tics.hpp>
#include <cmath>
#include <cassert>

#include <fmt/format.h>

using namespace std ;

namespace gfx {

std::string ScalarTickFormatter::format(double v, size_t) {
    return fmt::format("{:." + std::to_string(precision_) + "g}", v) ;
}

std::string FmtTickFormatter::format(double v, size_t) {
    return fmt::format(format_str_, v) ;
}


string FixedTickFormatter::format(double, size_t idx) {
    assert(idx < labels_.size());
    return labels_[idx] ;
}

double AutoTickLocator::sround(double x) {
    double s = 1.0 ;

    if ( fabs(x) < 1.0e-10 ) return 0.0 ;

    while ( x * s < 1.0 ) s *= 10 ;
    while ( x * s > 10.0 ) s /= 10 ;

    double sx = s * x ;

    if ( sx >= 1.0 && sx < 2.0 ) return 1.0/s ;
    else if ( sx >= 2.0 && sx < 5.0 ) return 2.0/s ;
    else return 5.0/s ;

    return s ;
}

void AutoTickLocator::bounds(double sx, double xu, double xl, unsigned &nTics, double &rxu, double &rxl)
{
    int i, n = (int)(xu/sx) ;

    for(i=n+1 ; i>=n-1 ; i--) {
        if ( i * sx - xu <= 1.0e-4 ) break ;
    }

    rxu = i * sx ;

    n = (int)(xl/sx) ;

    for(i=n-1 ; i<=n+1 ; i++) {
        if ( i * sx - xl >= -1.0e-4 ) break ;
    }

    rxl = i * sx ;

    nTics = (rxl - rxu)/sx + 1.5;
}


void AutoTickLocator::compute(double vmin, double vmax, double vscale, uint maxTics, double minStep, double &label_min, double &label_max, std::vector<double> &loc)
{
    vmin = vmin * vscale ; vmax = vmax * vscale ;
    uint tics = maxTics ;
    uint numTics ;
    double step ;

    label_max = vmax ;
    while (1)
    {
       step = sround((vmax - vmin)/(tics-1)) ;
       if ( step < minStep ) step = minStep ;

        bounds(step, vmin, vmax, numTics, label_min, label_max) ;

        if ( numTics <= maxTics ) break ;
        else tics -- ;

        if ( tics == 1 ) {
            numTics = 2 ;
            step = label_max - label_min ;
            break ;
        }
    }

    // step_ = sround(ticStep*vscale_) ;
    // bounds(step_, _min*vscale_, _max*vscale_, numTics, min_label_v_, max_label_v_) ;

    double v = label_min ;
    for( uint i=0 ; i<numTics ; i++ ) {
        loc.push_back(v) ;
        v += step ;
        if ( fabs(v) < 1.0e-4 ) v = 0.0 ;
    }
}

void FixedTickLocator::compute(double vmin, double vmax, double vscale, uint maxTics, double minStep, double &label_min, double &label_max, std::vector<double> &loc)
{
    label_min = vmin ;
    label_max = vmax ;

    for ( int i=0 ; i<loc_.size() ; i++ ) {
        label_min = std::min(label_min, loc_[i]) ;
        label_max = std::max(label_max, loc_[i]) ;
        loc.push_back(loc_[i]) ;
    }
}

void MultipleLocator::compute(double vmin, double vmax, double vscale, uint maxTics, double minStep, double &label_min, double &label_max, std::vector<double> &loc) {
    double base = base_ * vscale ;
    vmin = ceil( vmin * vscale / base ) * base ;
    vmax = floor( vmax * vscale / base ) * base ;

    for( double v = vmin - base ; v < vmax + base ; v += base ) {
        loc.push_back(v) ;
    }

    label_min = loc.front() ;
    label_max = loc.back() ;
}




}
