#pragma once

/* Standard Libraries */
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

/* User Libraries*/
#include "align_algo_parameters.h"
#include "dna_sequence.h"
#include "constants.h"

/* Using statements */
using std::string;
using std::fstream;
using std::ios_base;
using std::vector;
using std::streambuf;
using std::stringstream;

namespace GenomicsFileTypeParser
{
    AlgorithmParameters loadAlignAlgoParamsFromFile(string fileName);
    void loadDNASequenceToList(string fileName, vector<DNASequence>& seqList);
    void loadAlphabet(string fileName, string &alphabet);
}