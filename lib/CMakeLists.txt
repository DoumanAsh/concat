add_library(concat INTERFACE)
target_include_directories(concat INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}")
target_sources(concat INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}")
