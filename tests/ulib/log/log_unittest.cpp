#include <gtest/gtest.h>
#include "ulib/log/log.h"
#include <string.h>

static const char* kLogTag = "UnitTest";

class ULIB_LOG : public testing::Test {
protected:
    void SetUp() override {
        static const size_t full_file_path_len = strlen(__FILE__);
        static const size_t project_file_path_len = strlen("tests/ulib/log/log_test.cpp");
        ULOG_SET_STRIPPED_PREFIX_LEN(full_file_path_len - project_file_path_len);
    }
};

TEST_F(ULIB_LOG, base_test)
{
    ULOG_TRACE(kLogTag, "trace log");
    ULOG_DEBUG(kLogTag, "debug log");
    ULOG_INFO(kLogTag, "info log");
    ULOG_WARN(kLogTag, "warn log");
    ULOG_ERROR(kLogTag, "error log");
    ULOG_FATAL(kLogTag, "fatal log");
}
