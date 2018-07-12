cmake_minimum_required (VERSION 2.8.9)
project (asp CXX)

# release build:
set(CMAKE_BUILD_TYPE "Debug")

# require C++11
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "-fopenmp -g -Og")
message("CMAKE_CXX_FLAGS is ${CMAKE_CXX_FLAGS}")


add_library(myLib SHARED "")
include(src/CMakeLists.txt)
target_compile_options(myLib PUBLIC "-g")
install(TARGETS myLib DESTINATION lib)

add_library(testLib SHARED "")
include(tests/CMakeLists.txt)
target_compile_options(testLib PUBLIC "-g")
install(TARGETS testLib DESTINATION lib)

add_executable(main main.cpp)
target_compile_options(main PUBLIC "-g")
target_link_libraries(main myLib)
# target_link_libraries(main testLib)
install(TARGETS main DESTINATION build)

add_executable(tests main-tests.cpp)
target_link_libraries(tests myLib)
target_link_libraries(tests testLib)
install(TARGETS tests DESTINATION build)