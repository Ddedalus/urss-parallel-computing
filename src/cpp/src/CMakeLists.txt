target_include_directories(myLib PUBLIC "${PROJECT_SOURCE_DIR}/include")
set_target_properties(myLib PROPERTIES LINKER_LANGUAGE CXX)
file(GLOB LIB_SRC "${CMAKE_CURRENT_LIST_DIR}/*.cpp")

# message("Found sources for myLib: ${LIB_SRC}")
target_sources(myLib PRIVATE ${LIB_SRC})
