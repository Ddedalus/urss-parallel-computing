cmake_minimum_required (VERSION 2.8.9)
project (asp)

add_definitions("-fopenmp")


add_library(myLib graphIO.cpp algorithms.cpp Bitset.cpp)

include(tests/CMakeLists.txt)

install(TARGETS myLib DESTINATION lib)
install(FILES graphIO.h algorithms.h Bitset.h DESTINATION include)

add_executable(main main.cpp)
target_link_libraries(main myLib)
install(TARGETS main DESTINATION build)

# build additional binary from alternative version of the main file
# add_executable(bitset alt-main.cpp)
# target_link_libraries(bitset myLib)
# target_compile_options(bitset "-O2")
# install(TARGETS bitset DESTINATION bin)