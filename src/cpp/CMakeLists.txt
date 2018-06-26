cmake_minimum_required (VERSION 2.8.9)
project (hello)

file(GLOB SOURCES "tests/*.cpp")
# file(GLOB SOURCES "*.cpp")
add_executable(main ${SOURCES})



# add_library(GraphIO graphIO.cpp)

# target_link_libraries(ASP GraphIO)
