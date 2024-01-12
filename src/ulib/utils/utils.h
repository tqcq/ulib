#ifndef ULIB_SRC_ULIB_UTILS_UTILS_H_
#define ULIB_SRC_ULIB_UTILS_UTILS_H_

#include <functional>

namespace ulib {
template<typename T>
const T &
Clamp(const T &value, const T &low, const T &high)
{
    return Clamp(value, low, high, std::less<T>{});
}

template<typename T, typename Comp>
const T &
Clamp(const T &value, const T &low, const T &high, Comp comp)
{
    return comp(value, low) ? low : (comp(high, value) ? high : value);
}

}// namespace ulib
#endif//  ULIB_SRC_ULIB_UTILS_UTILS_H_
