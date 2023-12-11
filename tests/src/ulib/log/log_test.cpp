#include <gtest/gtest.h>
#include <ulib/log/log.h>

TEST(ULIB_LOG, base_test)
{
    ULOG_TRACE("trace log");
    ULOG_DEBUG("debug log");
    ULOG_INFO("info log");
    ULOG_WARN("warn log");
    ULOG_ERROR("error log");
    ULOG_FATAL("fatal log");
}
