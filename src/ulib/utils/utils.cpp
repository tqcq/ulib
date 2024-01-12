#include "utils.h"
#include <sstream>

namespace ulib {
std::string
StrJoin(std::vector<nonstd::string_view> &vec,
        nonstd::string_view delimiter,
        bool ignore_empty_str)
{
    if (vec.empty()) { return ""; }

    auto iter = vec.cbegin();
    if (ignore_empty_str) {
        // find first non-empty string
        while (iter != vec.cend() && iter->empty()) { ++iter; }
    }

    if (iter == vec.cend()) { return ""; }
    std::stringstream ss;
    ss << *iter;
    for (++iter; iter != vec.cend(); ++iter) {
        if (ignore_empty_str && iter->empty()) { continue; }
        ss << delimiter << *iter;
    }

    return std::move(ss.str());
}

}// namespace ulib
