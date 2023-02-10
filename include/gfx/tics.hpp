#ifndef GFX_PLOT_TICS_HPP
#define GFX_PLOT_TICS_HPP

#include <vector>
#include <functional>

namespace gfx {

class TickFormatter {
public:
    virtual std::string format(double v, size_t idx) = 0 ;
};

class ScalarTickFormatter: public TickFormatter {
public:
    ScalarTickFormatter(size_t precision = 2): precision_(precision) {}

    std::string format(double v, size_t idx) override ;
private:
    size_t precision_ ;
};

class FmtTickFormatter: public TickFormatter {
public:
    FmtTickFormatter(const std::string &format_str): format_str_(format_str) {}

    std::string format(double v, size_t idx) override ;

private:
    std::string format_str_ ;
};

class FixedTickFormatter: public TickFormatter {
public:
    FixedTickFormatter(const std::vector<std::string> &labels): labels_(labels) {}

    std::string format(double v, size_t idx) override ;

private:
    std::vector<std::string> labels_ ;
};

using TickFormatterFunc = std::function<std::string(double, size_t)> ;

class FuncTickFormatter: public TickFormatter {
public:
    FuncTickFormatter(TickFormatterFunc f): func_(f) {}

    std::string format(double v, size_t idx) override { return func_(v, idx) ; }
private:
    TickFormatterFunc func_ ;
};


class TickLocator {
public:
    // override to define custom tick positions
    virtual void compute(double vmin, double vmax, double vscale, uint maxTics, double minStep,
                         double &label_min, double &label_max, std::vector<double> &loc) = 0;
};

class AutoTickLocator: public TickLocator {
public:
    void compute(double vmin, double vmax, double vscale, uint maxTics, double minStep,
                 double &label_min, double &label_max, std::vector<double> &loc) override;

protected:
    // round to one decimal point
    static double sround(double x);
    // compute normalized coordinates and associated tics
    static void bounds(double sx, double xu, double xl, unsigned &nTics, double &rxu, double &rxl);
};


class FixedTickLocator: public TickLocator {
public:
    FixedTickLocator(const std::vector<double> &loc): loc_(loc) {}

    void compute(double vmin, double vmax,
                 double vscale, uint maxTics, double minStep, double &label_min, double &label_max, std::vector<double> &loc) override;

private:
    const std::vector<double> loc_ ;
};

class MultipleLocator: public TickLocator {
public:
    MultipleLocator(double base): base_(base) {}

    void compute(double vmin, double vmax,
                 double vscale, uint maxTics, double minStep, double &label_min, double &label_max, std::vector<double> &loc) override;

private:
    double base_ ;
};

}

#endif
