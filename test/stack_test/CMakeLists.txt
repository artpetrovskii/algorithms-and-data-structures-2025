enable_testing()

add_executable(
  stack_test
  stack_test.cc
)
target_link_libraries(
  stack_test
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(stack_test
    EXTRA_ARGS "--gtest_output=xml:../../../allure-results/xunit9.xml"
)
