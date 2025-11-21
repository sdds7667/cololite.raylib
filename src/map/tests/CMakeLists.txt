enable_testing()
include(GoogleTest)

## hex_coord_2_unit_tests
add_executable(map_hex_coord_2_unit_tests hex_coord_2_unit_tests.cc)
target_link_libraries(map_hex_coord_2_unit_tests PRIVATE map gtest_main)
gtest_discover_tests(map_hex_coord_2_unit_tests)

## map bounds tests:
add_executable(ut_map_bounds ut_map_bounds.cc)
target_link_libraries(ut_map_bounds PRIVATE map gtest_main)
gtest_discover_tests(ut_map_bounds)

add_executable(hex_map_creation hex_map_creation.cc)
target_link_libraries(hex_map_creation PRIVATE map gtest_main)
gtest_discover_tests(hex_map_creation)