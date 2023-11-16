//
// Created by tqcq on 2023/11/16.
//

#ifndef ULIB_SRC_BASE_CHECKS_H_
#define ULIB_SRC_BASE_CHECKS_H_

#include <ostream>

namespace tqcq {

namespace detail {
void CheckLog(const char* expr, const char* file, int line, const char* msg, ...);
} // namespace detail

#define ULIB_CHECK_IMPL(condition, file, line) ::tqcq::detail::CheckLog(condition, file, line, "")

#if !defined(NDEBUG) || defined(ULIB_DCHECK_ALWAYS_ON)
#define ULIB_DCHECK_IS_ON 1
#else
#define ULIB_DCHECK_IS_ON 0
#endif

#define ULIB_CHECK(condition) \
  (condition) ? static_cast<void>(0) : ULIB_CHECK_IMPL(#condition, __FILE__, __LINE__)
#define ULIB_CHECK_OP(val1, val2, op) \
  ((val1) op (val2)) ? static_cast<void>(0) : ULIB_CHECK_OP_IMPL(#val1 " " #op " " #val2, __FILE__, __LINE__)

#define ULIB_CHECK_EQ(val1, val2) ULIB_CHECK_OP(val1, val2, ==)
#define ULIB_CHECK_NE(val1, val2) ULIB_CHECK_OP(val1, val2, !=)
#define ULIB_CHECK_LE(val1, val2) ULIB_CHECK_OP(val1, val2, <=)
#define ULIB_CHECK_LT(val1, val2) ULIB_CHECK_OP(val1, val2, <)
#define ULIB_CHECK_GE(val1, val2) ULIB_CHECK_OP(val1, val2, >=)
#define ULIB_CHECK_GT(val1, val2) ULIB_CHECK_OP(val1, val2, >)

#if ULIB_DCHECK_IS_ON
#define ULIB_DCHECK(condition) ULIB_CHECK(condition)
#define ULIB_DCHECK_EQ(val1, val2) ULIB_CHECK_EQ(val1, val2)
#define ULIB_DCHECK_NE(val1, val2) ULIB_CHECK_NE(val1, val2)
#define ULIB_DCHECK_LE(val1, val2) ULIB_CHECK_LE(val1, val2)
#define ULIB_DCHECK_LT(val1, val2) ULIB_CHECK_LT(val1, val2)
#define ULIB_DCHECK_GE(val1, val2) ULIB_CHECK_GE(val1, val2)
#define ULIB_DCHECK_GT(val1, val2) ULIB_CHECK_GT(val1, val2)
#else // ULIB_DCHECK_IS_ON
#define ULIB_DCHECK(condition) static_cast<void>(0)
#define ULIB_DCHECK_EQ(val1, val2) static_cast<void>(0)
#define ULIB_DCHECK_NE(val1, val2) static_cast<void>(0)
#define ULIB_DCHECK_LE(val1, val2) static_cast<void>(0)
#define ULIB_DCHECK_LT(val1, val2) static_cast<void>(0)
#define ULIB_DCHECK_GE(val1, val2) static_cast<void>(0)
#define ULIB_DCHECK_GT(val1, val2) static_cast<void>(0)
#endif // ULIB_DCHECK_IS_ON

#define ULIB_RATAL() ULIB_CHECK(false)

} // namespacce tqcq

#endif //ULIB_SRC_BASE_CHECKS_H_
