cmake_minimum_required (VERSION 2.8.9)
project (hello)

file (GLOB CPP_FILES tests/*.cpp *.cpp)
file (GLOB H_FILES tests/*.h *.h)
set (SOURCE_FILES ${CPP_FILES} ${H_FILES})

include_directories("tests")
message(${SOURCE_FILES})
add_executable(main ${SOURCE_FILES})

# add_library(GraphIO graphIO.cpp)

# target_link_libraries(ASP GraphIO)
