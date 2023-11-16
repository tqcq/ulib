//
// Created by tqcq on 2023/11/16.
//

#include "system_time.h"
#include <sys/time.h>

namespace tqcq {
//TODO 后续使用系统相关API获取时间
int64_t SystemTimeNanos() {
  struct timeval tv;
  gettimeofday(&tv, nullptr);
  return tv.tv_sec * 1000000000LL + tv.tv_usec * 1000LL;
}

} // namespace tqcq
