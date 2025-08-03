#!/bin/sh
set -x
rm -rf build

rm allure-results/*

cmake -S . -B build
cmake --build build

./run_tests.sh

allure generate --clean ./allure-results -o ./allure-report
allure open ./allure-report
