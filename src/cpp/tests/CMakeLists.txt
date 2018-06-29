target_link_libraries(testLib PUBLIC myLib)

file(GLOB TEST_SRC "${CMAKE_CURRENT_LIST_DIR}/*.cpp")

# message("Found sources for testLib: ${TEST_SRC}")

target_sources(testLib PRIVATE ${TEST_SRC})
target_include_directories(testLib PUBLIC "${PROJECT_SOURCE_DIR}/include")

