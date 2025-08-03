enable_testing()

add_executable(
  new_test
  new_test.cc
)
target_link_libraries(
  new_test
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(new_test
    EXTRA_ARGS "--gtest_output=xml:../../../allure-results/xunit7.xml"
)
