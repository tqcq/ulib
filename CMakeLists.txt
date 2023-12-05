cmake_minimum_required(VERSION 3.10)

project(ulib LANGUAGES CXX VERSION 0.1.0)
option(ULIB_BUILD_TESTS "Build tests" OFF)

find_package(Threads REQUIRED)
set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(${PROJECT_NAME} STATIC ""
        src/base/location.cpp
        src/strings/string_printf.cpp
        src/system/system_time.cpp
        src/base/task_queue_base.cpp
        src/units/time_delta.cpp
        src/synchronization/mutex.cpp
        src/network/socket_factory.cpp
        src/network/socket_address.cpp
        src/network/socket_server.cpp
        src/synchronization/event.cpp
        src/logging/log.cpp
)
target_compile_definitions(${PROJECT_NAME} PRIVATE ULIB_LIBRARY_IMPL)
target_include_directories(${PROJECT_NAME} PUBLIC src)
target_sources(${PROJECT_NAME}
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src//base/checks.h
        ${CMAKE_CURRENT_SOURCE_DIR}/src//base/checks.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/types/optional.h
        ${CMAKE_CURRENT_SOURCE_DIR}/src/system/export_api.h
)


if (ULIB_BUILD_TESTS)
    find_package(GTest CONFIG REQUIRED)

    add_executable(ulib_test ""
            src/base/location_unittest.cpp)
    set_target_properties(ulib_test PROPERTIES
            CXX_STANDARD 11
            CXX_STANDARD_REQUIRED YES
            CXX_EXTENSIONS NO)

    target_link_libraries(ulib_test PRIVATE
            ulib
            GTest::gtest
            GTest::gtest_main
            GTest::gmock
            GTest::gmock_main
    )

    target_sources(ulib_test PRIVATE
            src/base/location_unittest.cpp
    )

    add_test(AllTestsInUlib ulib_test)
endif ()