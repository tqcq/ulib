#include <gtest/gtest.h>
#include <ulib/log/log.h>

static const char* kLogTag = "UnitTest";

TEST(ULIB_LOG_TEST, base_test)
{
    ULOG_TRACE(kLogTag, "trace log");
    ULOG_DEBUG(kLogTag, "debug log");
    ULOG_INFO(kLogTag, "info log");
    ULOG_WARN(kLogTag, "warn log");
    ULOG_ERROR(kLogTag, "error log");
    ULOG_FATAL(kLogTag, "fatal log");
}
