set(LFU_TEST LFU_test)
set(IDEAL_TEST ideal_test)

enable_testing()

add_executable(${LFU_TEST} LFU_test.cpp)
target_link_libraries(${LFU_TEST} PRIVATE gtest gtest_main)
target_include_directories(${LFU_TEST} PRIVATE ../../include)

add_executable(${IDEAL_TEST} ideal_test.cpp)
target_link_libraries(${IDEAL_TEST} PRIVATE gtest gtest_main)
target_include_directories(${IDEAL_TEST} PRIVATE ../../include)

include(GoogleTest)
gtest_discover_tests(${LFU_TEST})
gtest_discover_tests(${IDEAL_TEST})