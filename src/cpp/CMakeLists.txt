cmake_minimum_required (VERSION 2.8.9)
project (asp)

# release build:
set(DCMAKE_BUILD_TYPE "Release")

set(CMAKE_CXX_FLAGS "-fopenmp")
message("CMAKE_CXX_FLAGS is ${CMAKE_CXX_FLAGS}")

add_library(myLib graphIO.cpp algorithms.cpp Bitset.cpp)

include(tests/CMakeLists.txt)

install(TARGETS myLib DESTINATION lib)
install(FILES graphIO.h algorithms.h Bitset.h DESTINATION include)

add_executable(main main.cpp)
target_link_libraries(main myLib)
install(TARGETS main DESTINATION build)

add_executable(bitset bitset.cpp)
target_link_libraries(bitset myLib)
target_compile_options(bitset PUBLIC "-O2")
install(TARGETS bitset DESTINATION build)

# build additional binary from alternative version of the main file
# add_executable(bitset alt-main.cpp)
# target_link_libraries(bitset myLib)
# target_compile_options(bitset  PUBLIC -O2)
# install(TARGETS bitset DESTINATION bin)