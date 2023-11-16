//
// Created by tqcq on 2023/11/16.
//

#ifndef ULIB_SRC_BASE_LOCATION_H_
#define ULIB_SRC_BASE_LOCATION_H_
#include <string>
namespace tqcq {

class Location {
 public:
  Location();
  Location(const Location &other);
  Location(Location &&other);
  Location &operator=(const Location &other);

  static Location Current(const char *function_name = __builtin_FUNCTION(), const char *file_name = __builtin_FILE(), int line_number = __builtin_LINE());

  bool operator < (const Location& other) const {
    return program_counter_ < other.program_counter_;
  }

  bool operator == (const Location& other) const {
    return program_counter_ == other.program_counter_;
  }

  bool has_source_info() const { return function_name_ && file_name_; }
  const char *function_name() const { return function_name_; }
  const char *file_name() const { return file_name_; }
  int line_number() const { return line_number_; }
  const void *program_counter() const { return program_counter_; }

  std::string ToString() const;

 private:
  Location(const char *function_name, const char *file_name, int line_number, const void *program_counter);
  const char *function_name_ = nullptr;
  const char *file_name_ = nullptr;
  int line_number_ = -1;
  const void *program_counter_ = nullptr;
};

const void *GetProgramCounter();

#define FROM_HERE ::tqcq::Location::Current(__FUNCTION__, __FILE__, __LINE__)

} // namespace tqcq

#endif //ULIB_SRC_BASE_LOCATION_H_
