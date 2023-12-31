#ifndef GFX_SVG_LENGTH_HPP
#define GFX_SVG_LENGTH_HPP

#include <string>
#include "dictionary.hpp"

namespace svg {

enum class LengthUnitType { Unknown, Number, EMS, EXS, PX, CM, MM, IN, PT, PC, Percentage } ;
enum class LengthDirection { Absolute, Horizontal, Vertical, Viewport } ;

class Length
{
  public:

    Length(): unit_type_(LengthUnitType::Unknown), value_in_specified_units_(0) {}
    Length(float val, LengthUnitType unit = LengthUnitType::Number): unit_type_(unit), value_in_specified_units_(val) {
        if ( unit_type_ == LengthUnitType::Percentage ) value_in_specified_units_ /= 100.0 ;
    }

    bool parse(const std::string &str)  ;

    float value() const { return value_in_specified_units_ ; }

    LengthUnitType units() const { return unit_type_ ; }

private:
    LengthUnitType unit_type_ ;
    float value_in_specified_units_ ;
};

inline Length operator "" _cm( long double v ) {
  return Length{(float)v, LengthUnitType::CM};
}

inline Length operator "" _perc( long double v ) {
  return Length{(float)v, LengthUnitType::Percentage};
}

inline Length operator "" _px( long double v ) {
  return Length{(float)v, LengthUnitType::PX };
}

class LengthList {
public:

    LengthList() = default ;
    LengthList(const std::vector<Length> &ls): values_(ls) {}

    const std::vector<Length> &values() const { return values_ ; }

    bool parse(const std::string &str) ;

private:

    std::vector<Length> values_ ;
};

} // namespace svg



#endif

