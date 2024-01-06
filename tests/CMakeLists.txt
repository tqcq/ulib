set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_executable(ulib_test
        ulib/base/types_unittest.cpp
        ulib/log/log_unittest.cpp
        ulib/concorrency/mutex_unittest.cpp
        ulib/concorrency/event_unittest.cpp
        ulib/concorrency/countdown_latch_unittest.cpp
        ulib/system/thread_unittest.cpp
        ulib/system/thread_pool_unittest.cpp
        ulib/system/timer_unittest.cpp
        3party/inja/inja_unittest.cpp
        3party/optional/optional_unittest.cpp
        3party/sqlpp11/sqlpp11_unittest.cpp
        ulib/utils/defer_unittest.cpp
)
target_link_libraries(ulib_test PRIVATE
        ulib
        gtest
        gtest_main)

add_test(NAME ulib_test COMMAND ulib_test)
