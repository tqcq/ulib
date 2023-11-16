//
// Created by tqcq on 2023/11/16.
//

#ifndef ULIB_SRC_SYNCHRONIZATION_MUTEX_H_
#define ULIB_SRC_SYNCHRONIZATION_MUTEX_H_

#include <mutex>

namespace tqcq {

class Mutex final {
 public:
  Mutex() = default;
  Mutex(const Mutex &) = delete;
  Mutex &operator=(const Mutex &) = delete;

  void Lock() { mutex_.lock(); }
  bool TryLock() { return mutex_.try_lock(); }
  void Unlock() { mutex_.unlock(); }
  void AssertHeld() const {};
 private:
  std::mutex mutex_;
};

class MutexLock final {
 public:
  MutexLock(const MutexLock &) = delete;
  MutexLock &operator=(const MutexLock &) = delete;

  explicit MutexLock(Mutex *mu) : mu_(mu) { mu_->Lock(); }
  ~MutexLock() { mu_->Unlock(); }
 private:
  Mutex *const mu_;
};

} // namespace tqcq

#endif //ULIB_SRC_SYNCHRONIZATION_MUTEX_H_
