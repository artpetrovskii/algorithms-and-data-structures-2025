#!/bin/bash
mkdir -p build
cd build
cmake .. && make
# ./test/runTests
# allure generate --clean ../allure-results -o ../allure-report
# allure open ../allure-report
