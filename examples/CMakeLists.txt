cmake_minimum_required(VERSION 3.10)
project(ulib_examples LANGUAGES C CXX VERSION 0.1.0)

add_executable(ulib_examples
    main.cpp
)

target_link_libraries(ulib_examples PRIVATE ulib)

