cmake_minimum_required (VERSION 2.8.9)
project (asp CXX)

# release build:
set(CMAKE_BUILD_TYPE "Release")

# require C++11
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "-fopenmp")
message("CMAKE_CXX_FLAGS is ${CMAKE_CXX_FLAGS}")


add_library(myLib SHARED "")
include(src/CMakeLists.txt)
install(TARGETS myLib DESTINATION lib)

add_library(testLib SHARED "")
include(tests/CMakeLists.txt)

add_executable(main main.cpp)
target_compile_options(main PUBLIC "-O2")
target_link_libraries(main myLib)
install(TARGETS main DESTINATION build)

# add_executable(bitset bitset.cpp)
# target_link_libraries(bitset myLib)
# target_compile_options(bitset PUBLIC "-O2")
# install(TARGETS bitset DESTINATION build)

# build additional binary from alternative version of the main file
# add_executable(bitset alt-main.cpp)
# target_link_libraries(bitset myLib)
# target_compile_options(bitset  PUBLIC -O2)
# install(TARGETS bitset DESTINATION bin)