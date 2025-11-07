enable_testing()
include(GoogleTest)

## basic_add_tests
add_executable(map_unit_tests basic_add_tests.cc)
target_link_libraries(map_unit_tests PRIVATE map gtest_main)
gtest_discover_tests(map_unit_tests)

## hex_coord_2_unit_tests
add_executable(map_hex_coord_2_unit_tests hex_coord_2_unit_tests.cc)
target_link_libraries(map_hex_coord_2_unit_tests PRIVATE map gtest_main)
gtest_discover_tests(map_hex_coord_2_unit_tests)

