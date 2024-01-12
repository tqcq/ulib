cmake_minimum_required(VERSION 3.10)

project(
  ulib
  LANGUAGES CXX C
  VERSION 0.1.0)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(ULIB_BUILD_TESTS "Build tests" OFF)
option(ULIB_SHARED_LIB "Build shared library" OFF)
option(ULIB_BUILD_EXAMPLES "Build examples" OFF)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
    # set stripped
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -s")
endif()
# add -static-libgcc and -static-libstdc++ to link flags

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
if(ULIB_SHARED_LIB)
    add_library(${PROJECT_NAME} SHARED "")
else()
    add_library(${PROJECT_NAME} STATIC "" src/ulib/system/timer.cpp
                                     src/ulib/system/timer.h)
endif()
target_sources(
  ${PROJECT_NAME}
  PRIVATE 3party/mongoose/mongoose.c
          src/ulib/base/location.h
          src/ulib/base/location.cpp
          src/ulib/status.h
          src/ulib/status.cpp
          src/ulib/concorrency/barrier.cpp
          src/ulib/concorrency/barrier.h
          src/ulib/concorrency/mutex.cpp
          src/ulib/concorrency/mutex.h
          src/ulib/concorrency/condition_variable.cpp
          src/ulib/concorrency/condition_variable.h
          src/ulib/concorrency/countdown_latch.cpp
          src/ulib/concorrency/countdown_latch.h
          src/ulib/concorrency/internal/mutex_impl.cpp
          src/ulib/concorrency/internal/mutex_impl.h
          src/ulib/concorrency/internal/condition_variable_impl.cpp
          src/ulib/concorrency/internal/condition_variable_impl.h
          src/ulib/concorrency/event.cpp
          src/ulib/concorrency/event.h
          src/ulib/system/thread.h
          src/ulib/system/thread.cpp
          src/ulib/system/thread_pool.h
          src/ulib/system/thread_pool.cpp
          src/ulib/system/timer.h
          src/ulib/system/timer.cpp)

find_package(Threads REQUIRED)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# CMP0063
if(POLICY CMP0063)
    cmake_policy(SET CMP0063 NEW)
endif()
if(POLICY CMP0048)
    cmake_policy(SET CMP0048 NEW)
endif()

set(FMT_USE_CPP11
    OFF
    CACHE BOOL "Use C++11" FORCE)
set(FMT_TEST
    OFF
    CACHE BOOL "Build tests" FORCE)
set(FMT_USE_CPP11
    OFF
    CACHE BOOL "Use C++11" FORCE)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/3party/fmt)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/3party/sqlpp11)

set(JSONCPP_WITH_TESTS OFF)
set(JSONCPP_WITH_POST_BUILD_UNITTEST OFF)
set(JSONCPP_WITH_PKGCONFIG_SUPPORT OFF)
set(BUILD_SHARED_LIBS OFF)
set(BUILD_OBJECT_LIBS OFF)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/3party/jsoncpp)

target_sources(
  ${PROJECT_NAME} PRIVATE src/ulib/empty.cpp src/ulib/log/logger.cpp
                          src/ulib/log/log.cpp src/ulib/log/level.cpp)
target_link_libraries(${PROJECT_NAME} PUBLIC fmt::fmt jsoncpp_static
                                             sqlpp11::sqlpp11)
target_compile_definitions(${PROJECT_NAME} PRIVATE ULIB_LIBRARY_IMPL)
target_include_directories(
  ${PROJECT_NAME}
  PUBLIC 3party/inja
         3party/mongoose
         3party/nlohmann
         3party/nonstd
         3party/sigslot
         3party/rxcpp/Rx/v2/src
         3party/rxcpp/Ix/CPP/src
         src)

install(TARGETS ${PROJECT_NAME} DESTINATION lib)

if(ULIB_BUILD_TESTS)
    enable_testing()
    set(GTEST_LANG_CXX11 OFF)
    set(GTEST_HAS_TR1_TUPLE OFF)
    set(GTEST_USE_OWN_TR1_TUPLE ON)
    add_subdirectory(3party/googletest)
    add_subdirectory(tests)
endif()

if(ULIB_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()
