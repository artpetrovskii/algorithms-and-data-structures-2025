enable_testing()

add_executable(
  graph_test
  graph_test.cc
)
target_link_libraries(
  graph_test
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(graph_test
    EXTRA_ARGS "--gtest_output=xml:../../../allure-results/xunit12.xml"
)
