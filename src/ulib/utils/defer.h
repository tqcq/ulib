#ifndef ULIB_SRC_ULIB_UTILS_DEFER_H_
#define ULIB_SRC_ULIB_UTILS_DEFER_H_
#include "ulib/base/noncopyable.h"
#include "ulib/base/stringize_macros.h"
#include <memory>
#include <algorithm>
#include <vector>
#include <functional>

namespace ulib {
class Defer : NonCopyable {
public:
    using DeferFunc = std::function<void()>;

    Defer() = default;

    Defer(const DeferFunc &defer_func) { Add(defer_func); }

    Defer(DeferFunc &&defer_func) { Add(std::move(defer_func)); }

    Defer(Defer &&other) noexcept : defer_funcs_(std::move(other.defer_funcs_))
    {}

    ~Defer()
    {
        std::for_each(defer_funcs_.rbegin(), defer_funcs_.rend(),
                      [](const DeferFunc &defer_func) { defer_func(); });
    }

    template<typename F, typename... Args>
    void Add(F &&func, Args &&...args)
    {
        defer_funcs_.push_back(
            std::bind(std::forward<F>(func), std::forward<Args>(args)...));
    }

    void Clear() { defer_funcs_.clear(); }

private:
    std::vector<DeferFunc> defer_funcs_;
};

#define _ULIB_DEFER_CONCAT(a, b) a##b
#define ULIB_DEFER_CONCAT(a, b) _ULIB_DEFER_CONCAT(a, b)

#define defer(x)                                                               \
    ::ulib::Defer ULIB_DEFER_CONCAT(__ulib_defer__, __LINE__) =                \
        ::ulib::Defer(x);

}// namespace ulib
#endif// ULIB_SRC_ULIB_UTILS_DEFER_H_
