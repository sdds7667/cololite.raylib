enable_testing()
add_executable(map_unit_tests basic_add_tests.cc)
target_link_libraries(map_unit_tests PRIVATE map gtest_main)

include(GoogleTest)
gtest_discover_tests(map_unit_tests)