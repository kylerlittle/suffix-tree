#pragma once

/* Standard Libraries */
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <cstdlib>

/* User Libraries */
#include "constants.h"

/* Using statements */
using std::string;
using std::fstream;
using std::cout;
using std::endl;

class AlgorithmParameters
{
    public:
        /* Parameters */
        int matchScore;                 // match
        int mismatchScore;              // mismatch
        int gapOpeningPenalty;          // cost of opening gap (h)
        int gapContinuationPenalty;     // cost of continuing a gap (g)
};

/*
 * Overloads the Stream Extraction Operator >> for AlgorithmParameters object.
 * Good design because it leaves the fstream extraction details to the object that
 * it's mapping to (rather than having these in a higher level class).
 */
fstream & operator >> (fstream &lhs, AlgorithmParameters &rhs);

