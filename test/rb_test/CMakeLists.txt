enable_testing()

add_executable(
  rb_test
  rb_test.cc
)
target_link_libraries(
  rb_test
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(rb_test
    EXTRA_ARGS "--gtest_output=xml:../../../allure-results/xunit11.xml"
)
