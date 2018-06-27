cmake_minimum_required (VERSION 2.8.9)
project (asp)

add_library(myLib graphIO.cpp algorithms.cpp Bitset.cpp)

include(tests/CMakeLists.txt)

install(TARGETS myLib DESTINATION lib)
install(FILES graphIO.h algorithms.h Bitset.h DESTINATION include)

add_executable(bfs main.cpp)
target_link_libraries(bfs myLib)
install(TARGETS bfs DESTINATION build)

add_executable(bitset alt-main.cpp)
target_link_libraries(bitset myLib)
install(TARGETS bitset DESTINATION bin)