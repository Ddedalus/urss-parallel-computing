target_sources(myLib
    PRIVATE
        "${CMAKE_CURRENT_LIST_DIR}/tests.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/tBitset.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/tgraphIO.cpp"
    PUBLIC
        "${CMAKE_CURRENT_LIST_DIR}/tests.h"
)

target_include_directories(myLib PUBLIC "${CMAKE_CURRENT_LIST_DIR}")
