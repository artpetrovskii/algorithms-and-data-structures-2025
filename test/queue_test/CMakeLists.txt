enable_testing()

add_executable(
  queue_test
  queue_test.cc
)
target_link_libraries(
  queue_test
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(queue_test
    EXTRA_ARGS "--gtest_output=xml:../../../allure-results/xunit99.xml"
)
