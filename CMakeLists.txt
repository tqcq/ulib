cmake_minimum_required(VERSION 3.10)

project(ulib LANGUAGES CXX VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Threads REQUIRED)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

set(FMT_USE_CPP11 OFF)
set(FMT_TEST OFF)
set(FMT_USE_CPP11 OFF)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/3party/fmt)

option(ULIB_BUILD_TESTS "Build tests" OFF)

add_library(${PROJECT_NAME} STATIC
        src/ulib/empty.cpp
        src/ulib/log/logger.cpp
        src/ulib/log/log.cpp
        src/ulib/log/level.cpp
)
target_link_libraries(${PROJECT_NAME} PUBLIC fmt::fmt)
target_compile_definitions(${PROJECT_NAME} PRIVATE ULIB_LIBRARY_IMPL)
target_include_directories(${PROJECT_NAME} PUBLIC src)

add_executable(ulib_main_test src/main.cpp)
target_link_libraries(ulib_main_test PRIVATE ${PROJECT_NAME})

install(TARGETS ${PROJECT_NAME} DESTINATION lib)
