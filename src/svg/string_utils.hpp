#ifndef STRING_UTILS_HPP
#define STRING_UTILS_HPP

#include <string>
#include <regex>

inline bool ends_with(std::string const & value, std::string const & ending)
{
    if (ending.size() > value.size()) return false;
    return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
}

inline bool starts_with(std::string const & value, std::string const & ending) {
    return value.find(ending) == 0 ;
}

inline std::vector<std::string> split(const std::string &s, const std::regex &r) {
    std::sregex_token_iterator it{s.begin(), s.end(), r, -1};
    return {it, {}};
}


inline std::string trim(const std::string &src) {
    size_t first = src.find_first_not_of(' ');
    if ( first == std::string::npos ) return src ;

    size_t last = src.find_last_not_of(' ');
    return src.substr(first, (last - first + 1));
}

#endif
