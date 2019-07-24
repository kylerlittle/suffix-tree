#pragma once

/* Standard Libraries */
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cctype>

/* User Libraries */
#include "align_algo_parameters.h"

/* Using statements */
using std::string;
using std::fstream;
using std::iostream;
using std::stringstream;
using std::toupper;

/* Convert string to uppercase in place. */
void string_to_upper(string &s);

class DNASequence
{
    public:
        /* Constructor */
        DNASequence();

        /* Members Variables */
        string sequence;
        string id;
        string description;
};

/* Overloads the IOStream Extraction Operator >> for DNASequence object */
iostream & operator >> (iostream &lhs, DNASequence &rhs);