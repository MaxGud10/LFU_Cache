cmake_minimum_required (VERSION 3.8 FATAL_ERROR)

project (cache)

add_executable (${PROJECT_NAME})

add_subdirectory (src)
target_include_directories (${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)