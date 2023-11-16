//
// Created by tqcq on 2023/11/16.
//

#ifndef ULIB_SRC_STRINGS_STRING_PRINTF_H_
#define ULIB_SRC_STRINGS_STRING_PRINTF_H_
#include <string>
#include <stdarg.h>

namespace tqcq {

std::string StringPrintf(const char* format, ...);
void StringAppendF(std::string* dst, const char* format, ...);
void StringAppendV(std::string* dst, const char* format, va_list ap);

} // namespace tqcq

#endif //ULIB_SRC_STRINGS_STRING_PRINTF_H_
