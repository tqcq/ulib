# set cross-compiled system type, it's better not use the type which cmake cannot recognized.
set(CMAKE_CROSSCOMPILING TRUE)
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR arm)
# when hislicon SDK was installed, toolchain was installed in the path as below:
set(CMAKE_C_COMPILER "/opt/hisi-linux/x86-arm/arm-himix200-linux/bin/arm-himix200-linux-gcc")
set(CMAKE_CXX_COMPILER "/opt/hisi-linux/x86-arm/arm-himix200-linux/bin/arm-himix200-linux-g++")
set(CMAKE_FIND_ROOT_PATH "/opt/hisi-linux/x86-arm/arm-himix200-linux")

# set searching rules for cross-compiler
if(NOT CMAKE_FIND_ROOT_PATH_MODE_PROGRAM)
    set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
endif()
if(NOT CMAKE_FIND_ROOT_PATH_MODE_LIBRARY)
    set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
endif()
if(NOT CMAKE_FIND_ROOT_PATH_MODE_INCLUDE)
    set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
endif()
if(NOT CMAKE_FIND_ROOT_PATH_MODE_PACKAGE)
    set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
endif()

# set ${CMAKE_C_FLAGS} and ${CMAKE_CXX_FLAGS}flag for cross-compiled process
set(CROSS_COMPILATION_ARM himix200)
set(CROSS_COMPILATION_ARCHITECTURE armv7-a)

# set g++ param
set(CMAKE_CXX_FLAGS "-march=armv7-a -mfloat-abi=softfp -mfpu=neon-vfpv4 ${CMAKE_CXX_FLAGS}")

# cache flags
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}" CACHE STRING "c flags")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}" CACHE STRING "c++ flags")