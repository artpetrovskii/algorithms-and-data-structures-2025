#!/bin/sh
set -x
rm -rf build

rm allure-results/*

cmake -S . -B build
cmake --build build
cd build/test/basic_test && ctest
cd ../../..
cd build/test/sample_test && ctest
cd ../../..
cd build/test/new_test && ctest
cd ../../..
cd build/test/stack_test && ctest
cd ../../..
cd build/test/queue_test && ctest
cd ../../..
cd build/test/bst_test && ctest
cd ../../..
cd build/test/avl_test && ctest
cd ../../..

allure generate --clean ./allure-results -o ./allure-report
allure open ./allure-report
