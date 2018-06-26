cmake_minimum_required (VERSION 2.8.9)
project (hello)

file (GLOB CPP_FILES test/*.cpp *.cpp)
file (GLOB H_FILES *.h /tests/*.h)
set (SOURCE_FILES ${CPP_FILES} ${H_FILES})

file(GLOB SOURCES *.cpp tests/*.cpp)
add_executable(main ${SOURCE_FILES})



# add_library(GraphIO graphIO.cpp)

# target_link_libraries(ASP GraphIO)
