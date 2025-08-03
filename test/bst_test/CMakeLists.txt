enable_testing()

add_executable(
  bst_test
  bst_test.cc
)
target_link_libraries(
  bst_test
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(bst_test
    EXTRA_ARGS "--gtest_output=xml:../../../allure-results/xunit999.xml"
)
