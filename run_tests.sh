#!/bin/sh
set -x

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
cd build/test/table_test && ctest
cd ../../..
cd build/test/bst_test && ctest
cd ../../..
cd build/test/avl_test && ctest
cd ../../..
cd build/test/rb_test && ctest
cd ../../..
cd build/test/b_test && ctest
cd ../../..
cd build/test/graph_test && ctest
cd ../../..
