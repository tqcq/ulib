//
// Created by tqcq on 2023/11/16.
//

#ifndef ULIB_SRC_UNITS_TIME_DELTA_H_
#define ULIB_SRC_UNITS_TIME_DELTA_H_

#include <cstdint>
#include <type_traits>
#include <string>
#include "units/unit_base.h"

namespace tqcq {

class TimeDelta final : public RelativeUnit<TimeDelta>{
 public:
  TimeDelta() = delete;
  template<typename T>
  static constexpr TimeDelta Minutes(T value) {
    static_assert(std::is_arithmetic<T>::value, "");
    return Seconds(value * 60);
  }

  template<typename T>
  static constexpr TimeDelta Seconds(T value) {
    static_assert(std::is_arithmetic<T>::value, "");
    return TimeDelta(value * 1000000);
  }

  template<typename T>
  static constexpr TimeDelta Millis(T value) {
    static_assert(std::is_arithmetic<T>::value, "");
    return TimeDelta(value * 1000);
  }

  template<typename T>
  static constexpr TimeDelta Micros(T value) {
    static_assert(std::is_arithmetic<T>::value, "");
    return FromValue(value);
  }

  static constexpr TimeDelta Max() {
    return TimeDelta::Micros(std::numeric_limits<int64_t>::max());
  }

  template<typename T = int64_t>
  constexpr T seconds() const { return value_ / 1000000; }

  template<typename T = int64_t>
  constexpr T ms() const { return value_ / 1000; }

  template<typename T = int64_t>
  constexpr T us() const { return value_; }

  template<typename T = int64_t>
  constexpr T ns() const { return value_ * 1000; }

  constexpr TimeDelta Abs() const {
    return us() < 0 ? TimeDelta::Micros(-us()) : *this;
  }


 private:
  friend class UnitBase<TimeDelta>;
  int64_t value_;
  using RelativeUnit::RelativeUnit;
  static constexpr bool one_sided = false;
};

std::string ToString(const TimeDelta &value);
inline std::string ToString(const TimeDelta &value) {
  return std::to_string(value.us());
}

inline std::ostream& operator<<(std::ostream& stream, const TimeDelta& value) {
  return stream << ToString(value);
}

} // namespace tqcq

#endif //ULIB_SRC_UNITS_TIME_DELTA_H_
