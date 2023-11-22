//
// Created by tqcq on 2023/11/22.
//

#ifndef ULIB_SRC_COMMON_PLATFORM_H_
#define ULIB_SRC_COMMON_PLATFORM_H_

#define ULIB_PLATFORM_WINDOWS   1
#define ULIB_PLATFORM_UNIX      2
#define ULIB_PLATFORM_APPLE     3
#define ULIB_PLATFORM_INTEL     4

#if defined(_WIN64)
#   define ULIB_PLATFORM ULIB_PLATFORM_WINDOWS
#elif defined(__WIN32__) || defined(WIN32) || defined(_WIN32)
#   define ULIB_PLATFORM ULIB_PLATFORM_WINDOWS
#elif defined(__APPLE_CC__)
#   define ULIB_PLATFORM ULIB_PLATFORM_APPLE
#elif defined(__INTEL_COMPILER)
#   define ULIB_PLATFORM ULIB_PLATFORM_INTEL
#else
#   define ULIB_PLATFORM ULIB_PLATFORM_UNIX
#endif

#define ULIB_COMPILER_GNU 1
#define ULIB_COMPILER_CLANG 2

#ifdef __GUNC__
#   define ULIB_COMPILER ULIB_COMPILER_GNU
#elif __clang__
#   define ULIB_COMPILER ULIB_COMPILER_CLANG
#else
#   error "FATAL ERROR: Unknown compiler."
#endif

namespace tqcq {

class CompilerDefs {
 public:
                        static bool IsWindows() { return ULIB_PLATFORM == ULIB_PLATFORM_WINDOWS; }
                        static bool IsUnix() { return ULIB_PLATFORM == ULIB_PLATFORM_UNIX; }
                        static bool IsApple() { return ULIB_PLATFORM == ULIB_PLATFORM_APPLE; }
                        static bool IsIntel() { return ULIB_PLATFORM == ULIB_PLATFORM_INTEL; }

                        static bool IsGNU() { return ULIB_COMPILER == ULIB_COMPILER_GNU; }
                        static bool IsClang() { return ULIB_COMPILER == ULIB_COMPILER_CLANG; }
};

} // namespace tqcq

#endif //ULIB_SRC_COMMON_PLATFORM_H_
