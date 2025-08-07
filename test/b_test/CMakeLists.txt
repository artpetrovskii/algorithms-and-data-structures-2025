enable_testing()

add_executable(
  b_test
  b_test.cc
)
target_link_libraries(
  b_test
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(b_test
    EXTRA_ARGS "--gtest_output=xml:../../../allure-results/xunit10.xml"
)
