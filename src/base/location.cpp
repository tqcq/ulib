//
// Created by tqcq on 2023/11/16.
//

#include "location.h"
#include "strings/string_printf.h"

namespace tqcq {

#if defined(COMPILER_MSVC)
#define RETURN_ADDRESS() _ReturnAddress()
#elif defined(COMPILER_GCC)
#define RETURN_ADDRESS() \
  __builtin_extract_return_addr(__builtin_return_address(0))
#else
#define RETURN_ADDRESS() nullptr
#endif

template<char ch, int len, const char *str>
struct StrLenImpl {
  static constexpr int value = StrLenImpl<str[len], len + 1, str>::value;
};

template<int len, const char *str>
struct StrLenImpl<'\0', len, str> {
  static constexpr int value = len;
};

template<const char* str>
struct StrLen {
  static constexpr int value = StrLenImpl<str[0], 0, str>::value;
};

template<>
struct StrLen<nullptr> {
  static constexpr int value = 0;
};


// calc stripped length
constexpr char full_path[] = __FILE__;
#if defined(__clang__) && defined(_MSC_VER)
constexpr char stripped[] = "base\\location.cpp";
#else
constexpr char stripped[] = "base/location.cpp";
#endif

static_assert(StrLen<full_path>::value >= StrLen<stripped>::value,
              "The file name does not match the expected prefix format.");
constexpr size_t kStrippedFilePathPrefixLength= StrLen<full_path>::value - StrLen<stripped>::value;

Location::Location() = default;
Location::Location(const Location &other) = default;
Location::Location(Location &&other) = default;
Location &Location::operator=(const Location &other) = default;

Location Location::Current(const char *function_name, const char *file_name, int line_number) {
  return Location(function_name, file_name + kStrippedFilePathPrefixLength, line_number, RETURN_ADDRESS());
}

std::string Location::ToString() const {
  if (has_source_info()) {
    return std::string(function_name_) + "@" + file_name_ + ":" + std::to_string(line_number_);
  }

  return StringPrintf("pc:%p", program_counter_);
}

Location::Location(const char *function_name, const char *file_name, int line_number, const void *program_counter)
    : function_name_(function_name),
      file_name_(file_name),
      line_number_(line_number),
      program_counter_(program_counter) {
}

const void *GetProgramCounter() {
  return RETURN_ADDRESS();
}

} // namespace tqcq
