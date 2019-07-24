#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <unordered_map>

#include "src/suffix_tree.h"
#include "src/genomics_file_type_parser.h"
#include "src/dna_sequence.h"
#include "src/constants.h"

using std::string;
using std::vector;
using std::cout;
using std::endl;
using std::fstream;
using std::unordered_map;

/*
 * Return BWT string from test file.
 */ 
string parseBWTFile(string bwtFileName);

/*
 * Return BWT string from generated SuffixTree
 */ 
string getBWT(string inputFile, string alphabetFile);

unordered_map<int, string> getIDPathLabelPairs(string inputFile, string alphabetFile);
