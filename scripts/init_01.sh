#!/bin/bash
mkdir -p {src,test,scripts,third_party,allure-results}
cat > CMakeLists.txt <<EOL
cmake_minimum_required(VERSION 3.10)
project(CppTestFramework)

# Download GoogleTest
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.12.1
)
FetchContent_MakeAvailable(googletest)

# Add test subdirectory
add_subdirectory(test)
EOL

cat > test/CMakeLists.txt <<EOL
add_executable(runTests
    test_sample.cpp
)

target_link_libraries(runTests
    gtest_main
)

# Generate Allure-compatible XML
gtest_discover_tests(runTests
    EXTRA_ARGS "--gtest_output=xml:allure-results/xunit.xml"
)
EOL

cat > test/test_sample.cpp <<EOL
#include <gtest/gtest.h>

TEST(SampleTest, BasicAssertion) {
    EXPECT_EQ(7 * 6, 42);
}
EOL

cat > scripts/run_01_tests.sh <<EOL
#!/bin/bash
mkdir -p build
cd build
cmake .. && make
./test/runTests
allure generate --clean ../allure-results -o ../allure-report
allure open ../allure-report
EOL

chmod +x scripts/*.sh
echo "✅ Framework initialized! Run: scripts/run_01_tests.sh"