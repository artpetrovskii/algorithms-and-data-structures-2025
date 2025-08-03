enable_testing()

add_executable(
  sample_test
  sample_test.cc
)
target_link_libraries(
  sample_test
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(sample_test
    EXTRA_ARGS "--gtest_output=xml:../../../allure-results/xunit2.xml"
)
