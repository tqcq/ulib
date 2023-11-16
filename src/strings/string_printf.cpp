//
// Created by tqcq on 2023/11/16.
//

#include "string_printf.h"
#include <stdarg.h>
#include <vector>
#include "base/scoped_clear_last_error.h"

namespace tqcq {
std::string StringPrintf(const char *format, ...) {
  va_list ap;
  va_start(ap, format);
  std::string result;
  StringAppendV(&result, format, ap);
  va_end(ap);

  return result;
}

void StringAppendF(std::string *dst, const char *format, ...) {
  va_list ap;
  va_start(ap, format);
  StringAppendV(dst, format, ap);
  va_end(ap);
}

template<typename CharT>
void StringAppendV(std::basic_string<CharT> *dst, const char *format, va_list ap) {
  // 32MB stack buffer, if the string is larger than this, failed
  const size_t kMaxBufferSize = 32 * 1024 * 1024;
  CharT stack_buf[1024];

  va_list ap_copy;
  va_copy(ap_copy, ap);

  ScopedClearLastError last_error;
  int result = vsnprintf(stack_buf, sizeof(stack_buf), format, ap_copy);
  va_end(ap_copy);

  if (result >= 0 && static_cast<size_t>(result) < sizeof(stack_buf)) {
    dst->append(stack_buf, static_cast<size_t>(result));
    return;
  }

  size_t mem_length = sizeof(stack_buf);
  while (true) {
    if (result < 0) {
      if (errno != 0 && errno != EOVERFLOW) {
        return;
      }

      mem_length *= 2;
    } else {
      // increase 1 characters
      mem_length = static_cast<size_t>(result) + 1;
    }

    if (mem_length > kMaxBufferSize) {
      // log
      return;
    }

    std::vector<CharT> mem_buf(mem_length);
    va_copy(ap_copy, ap);
    result = vsnprintf(mem_buf.data(), mem_length, format, ap_copy);
    va_end(ap_copy);

    if (result >= 0 && static_cast<size_t>(result) < mem_length) {
      dst->append(mem_buf.data(), static_cast<size_t>(result));
      return;
    }
  }
}

void StringAppendV(std::string *dst, const char *format, va_list ap) {
  StringAppendV<char>(dst, format, ap);
}

} // namespace tqcq
