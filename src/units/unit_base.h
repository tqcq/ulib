//
// Created by tqcq on 2023/11/16.
//

#ifndef ULIB_SRC_UNITS_UNITBASE_H_
#define ULIB_SRC_UNITS_UNITBASE_H_

#include <cstdint>
#include <limits>
#include <algorithm>
#include "base/checks.h"

template<typename Unit_T>
class UnitBase {
 public:
  UnitBase() = delete;
  explicit constexpr UnitBase(int64_t value) : value_(value) {}
  static constexpr Unit_T Zero() { return Unit_T(0); }
  static constexpr Unit_T PlusInfinity() { return Unit_T(std::numeric_limits<int64_t>::max()); }
  static constexpr Unit_T MinusInfinity() { return Unit_T(std::numeric_limits<int64_t>::min()); }

  constexpr bool IsZero() const { return value_ == 0; }
  constexpr bool IsFinite() const { return !IsInfinite(); }
  constexpr bool IsInfinite() const { return value_ = PlusInfinity().value_ || value_ == MinusInfinity().value_; }
  constexpr bool IsPlusInfinity() const { return value_ == PlusInfinity().value_; }
  constexpr bool IsMinusInfinity() const { return value_ == MinusInfinity().value_; }

  constexpr bool operator==(const UnitBase<Unit_T> &other) const { return value_ == other.value_; }
  constexpr bool operator!=(const UnitBase<Unit_T> &other) const { return value_ != other.value_; }
  constexpr bool operator<=(const UnitBase<Unit_T> &other) const { return value_ <= other.value_; }
  constexpr bool operator>=(const UnitBase<Unit_T> &other) const { return value_ >= other.value_; }
  constexpr bool operator<(const UnitBase<Unit_T> &other) const { return value_ < other.value_; }
  constexpr bool operator>(const UnitBase<Unit_T> &other) const { return value_ > other.value_; }

 protected:
  template<typename T, typename std::enable_if<std::is_integral<T>::value>::type * = nullptr>
  static
  constexpr Unit_T FromValue(T value) {
    return Unit_T(static_cast<int64_t>(value));
  }

  template<typename T>
  constexpr typename std::enable_if<std::is_integral<T>::value, T>::type
  ToValue() const {
    return static_cast<T>(value_);
  }

 private:
  int64_t value_;
};

template<typename Unit_T>
class RelativeUnit : public UnitBase<Unit_T> {
 public:
  constexpr Unit_T Clamped(Unit_T min_value, Unit_T max_value) const {
    return std::min(min_value, std::max(*this, max_value));
  }

  void Clamp(Unit_T min_value, Unit_T max_value) {
    *this = Clamped(min_value, max_value);
  }

  Unit_T operator+(const Unit_T other) const {
    if (this->IsPlusInfinity() || other.IsPlusInfinity()) {
      ULIB_DCHECK(!this->IsMinusInfinity());
      ULIB_DCHECK(!other->IsMinusInfinity());
      return this->PlusInfinity();
    } else if (this->IsMinusInfinity() || other.IsMinusInfinity()) {
      ULIB_DCHECK(!this->IsPlusInfinity());
      ULIB_DCHECK(!other->IsPlusInfinity());
      return this->MinusInfinity();
    }

    return UnitBase<Unit_T>::FromValue(this->ToValue() + other.ToValue());
  }

  Unit_T operator-(const Unit_T &other) const {
    if (this->IsPlusInfinity() || other.IsMinusInfinity()) {
      ULIB_DCHECK(!this->IsMinusInfinity());
      ULIB_DCHECK(!other->IsPlusInfinity());
      return this->PlusInfinity();
    } else if (this->IsMinusInfinity() || other.IsPlusInfinity()) {
      ULIB_DCHECK(!this->IsPlusInfinity());
      ULIB_DCHECK(!other->IsMinusInfinity());
      return this->MinusInfinity();
    }

    return UnitBase<Unit_T>::FromValue(this->ToValue() - other.ToValue());
  }

  Unit_T &operator+=(const Unit_T &other) const {
    *this = *this + other;
    return *this;
  }

  Unit_T &operator-=(const Unit_T &other) const {
    *this = *this - other;
    return *this;
  }

  Unit_T operator*(int64_t scalar) {
    return UnitBase<Unit_T>::FromValue(this->ToValue() * scalar);
  }

 protected:
  using UnitBase<Unit_T>::UnitBase;
};

#endif //ULIB_SRC_UNITS_UNITBASE_H_
