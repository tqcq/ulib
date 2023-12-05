set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(ulib_test
        src/ulib/sigslot/sigslot_test.cpp)

target_link_libraries(ulib_test PRIVATE
        ulib
        gtest
        gtest_main)

add_test(NAME ulib_test COMMAND ulib_test)
