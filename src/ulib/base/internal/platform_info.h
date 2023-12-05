//
// Created by tqcq on 2023/12/5.
//

#ifndef ULIB_SRC_BASE_INTERNAL_PLATFORM_INFO_H_
#define ULIB_SRC_BASE_INTERNAL_PLATFORM_INFO_H_
#include <stdint.h>

#if INTPTR_MAX == INT32_MAX
#   define ULIB_ARCH_32_BITS
#elif INTPTR_MAX == INT64_MAX
#   define ULIB_ARCH_64_BITS
#else
#error "Environment not 32 or 64-bit."
#endif

#endif//ULIB_SRC_BASE_INTERNAL_PLATFORM_INFO_H_
