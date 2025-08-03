enable_testing()

add_executable(
  avl_test
  avl_test.cc
)
target_link_libraries(
  avl_test
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(avl_test
    EXTRA_ARGS "--gtest_output=xml:../../../allure-results/xunit9999.xml"
)
