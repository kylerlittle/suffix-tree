#!/bin/bash

export TARGET_EXEC=SuffixTreeTests

g++ -std=c++11 src/*.cpp test_helper.cpp suffix_tree_tests.cpp gtest_main.cpp -lgtest -lpthread -o "${TARGET_EXEC}"
./$TARGET_EXEC