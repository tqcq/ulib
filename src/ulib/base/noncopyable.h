#ifndef ULIB_SRC_ULIB_BASE_NONCOPYABLE_H_
#define ULIB_SRC_ULIB_BASE_NONCOPYABLE_H_

namespace ulib {
class NonCopyable {
protected:
    NonCopyable() {}

    ~NonCopyable() {}

private:
    NonCopyable(const NonCopyable &);
    NonCopyable &operator=(NonCopyable &&);
};
}// namespace ulib
#endif// ULIB_SRC_ULIB_BASE_NONCOPYABLE_H_
