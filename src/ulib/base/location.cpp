#include "location.h"
#include <sstream>

namespace ulib {
#if defined(__clang__) && defined(_MSC_VER)
const char stripped[] = "ulib\\base\\location.cpp";
#else
const char stripped[] = "ulib/base/location.cpp";
#endif

#if !defined(ULIB_LOCATION_STRIPPED_PREFIX_LENGTH)                             \
    && !defined(ULIB_PROJECT_ROOT_PATH)
inline size_t
StrippedFilePathPrefixLength()
{
    const char path[] = __FILE__;
    static size_t stripped_file_path_prefix_length =
        sizeof(path) - sizeof(stripped);
    return stripped_file_path_prefix_length;
}
#elif defined(ULIB_LOCATION_STRIPPED_PREFIX_LENGTH)
inline size_t
StrippedFilePathPrefixLength()
{
    return ULIB_LOCATION_STRIPPED_PREFIX_LENGTH;
}
#else
inline size_t
StrippedFilePathPrefixLength()
{
    return sizeof(ULIB_PROJECT_ROOT_PATH);
}
#endif

Location::Location() = default;

Location::Location(const Location &other)
    : function_name_(other.function_name_),
      file_name_(other.file_name_),
      line_number_(other.line_number_)
{}

Location &
Location::operator=(const Location &other)
{
    function_name_ = other.function_name_;
    file_name_ = other.file_name_;
    line_number_ = other.line_number_;
    return *this;
}

std::string
Location::ToString() const
{
    std::stringstream ss;
    ss << function_name_ << "@" << file_name_ << ":" << line_number_;
    return ss.str();
}

}// namespace ulib
