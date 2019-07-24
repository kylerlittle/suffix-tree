#include "align_algo_parameters.h"

fstream & operator >> (fstream &lhs, AlgorithmParameters &rhs)
{
	char line[MAX_LINE], *s;
	string attributeName;
	int score;

	while (lhs.getline(line, MAX_LINE)) {
		/* Tokenize 'line' into attributeName (string) and score (int). */
		s = strtok(line, " ");
		attributeName = string(s);
		s = strtok(NULL, " ");
		score = atoi(s);

		/* Set parameter appropriately. Sadly, no C++ switch on strings :'( */
		if (attributeName == "match") {
			rhs.matchScore = score;
			cout << "match = " << rhs.matchScore << ", ";
		} else if (attributeName == "mismatch") { 
			rhs.mismatchScore = score;
			cout << "mismatch = " << rhs.mismatchScore << ", ";
		} else if (attributeName == "h") {
			rhs.gapOpeningPenalty = score;
			cout << "h = " << rhs.gapOpeningPenalty << ", ";
		} else if (attributeName == "g") {
			rhs.gapContinuationPenalty = score;
			cout << "g = " << rhs.gapContinuationPenalty << endl;
		} else {
			cout << "align_algo_parameters.cpp -- bad attribute name" << endl;
		}
	}
	
	return lhs;
}