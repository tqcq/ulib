//
// Created by tqcq on 2023/11/16.
//

#ifndef ULIB_SRC_BASE_SCOPED_CLEAR_LAST_ERROR_H_
#define ULIB_SRC_BASE_SCOPED_CLEAR_LAST_ERROR_H_
#include <errno.h>
namespace tqcq {

class ScopedClearLastError {
 public:
  ScopedClearLastError() : last_errno_(errno) { errno = 0; }
  ScopedClearLastError(const ScopedClearLastError &) = delete;
  ScopedClearLastError &operator=(const ScopedClearLastError &) = delete;
  ~ScopedClearLastError() { errno = last_errno_; }
 private:
  const int last_errno_;
};

} // namespace tqcq

#endif //ULIB_SRC_BASE_SCOPED_CLEAR_LAST_ERROR_H_
