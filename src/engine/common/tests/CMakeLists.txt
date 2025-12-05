enable_testing()
include(GoogleTest)

## LayeredContainer unit tests
add_executable(layered_container_tests layered_container_tests.cc)
target_link_libraries(layered_container_tests PRIVATE engine gtest_main)
gtest_discover_tests(layered_container_tests)
