#ifndef ULIB_SRC_ULIB_BASE_LOCATION_H_
#define ULIB_SRC_ULIB_BASE_LOCATION_H_

#include "types.h"
#include "stringize_macros.h"
#include <string>

namespace ulib {
class Location {
public:
    Location();
    Location(const Location &other);
    Location &operator=(const Location &other);

    std::string ToString() const;

    const char *function_name() const { return function_name_; }

    const char *file_name() const { return file_name_; }

    int line_number() const { return line_number_; }

    static Location Current(const char *function_name = __builtin_FUNCTION(),
                            const char *file_name = __builtin_FILE(),
                            int line_number = __builtin_LINE());

private:
    Location(const char *function_name, const char *file_name, int line_number);
    const char *function_name_ = nullptr;
    const char *file_name_ = nullptr;
    int line_number_ = -1;
};

#define ULIB_FROM_HERE ::ulib::Location::Current()

}// namespace ulib
#endif// ULIB_SRC_ULIB_BASE_LOCATION_H_
