#ifndef ULIB_SRC_ULIB_UTILS_UTILS_H_
#define ULIB_SRC_ULIB_UTILS_UTILS_H_

#include <functional>
#include <string_view.hpp>
#include <type_traits>
#include <vector>

namespace ulib {
template<typename T, class Comp>
const T &
Clamp(const T &value, const T &low, const T &high, Comp comp)
{
    return comp(value, low) ? low : (comp(high, value) ? high : value);
}

template<typename T>
const T &
Clamp(const T &value, const T &low, const T &high)
{
    return Clamp(value, low, high, std::less<T>{});
}

std::string StrJoin(std::vector<nonstd::string_view> &vec,
                    nonstd::string_view delimiter = ",",
                    bool ignore_empty_str = true);

}// namespace ulib
#endif//  ULIB_SRC_ULIB_UTILS_UTILS_H_
