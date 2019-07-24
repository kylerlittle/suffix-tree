/* Standard Libraries */
#include <iostream>
#include <string>
#include <stdlib.h>
#include <vector>
#include <time.h>

/* User-defined Libraries */
#include "genomics_file_type_parser.h"
#include "dna_sequence.h"
#include "suffix_tree.h"

/* Using statements */
using std::cout;
using std::endl;
using std::vector;

int main(int argc, char *argv[]) {
    string inputfile, alphabetfile;
    /* 
    * usage:
    *   $ <executable name> <input file containing both s1 and s2> <input alphabet file> 
    */
    if (argc < 2) {
        cout << "usage: $ <executable name> <input file containing both s1 and s2> <input alphabet file>" << endl;
        exit(0);
    }

    cout << "parameters passed: " << argv[1] << " " << argv[2] << endl;
    inputfile = argv[1];
    alphabetfile = argv[2];

    /* Next, extract Parameters */

    /* Step #1. Get a list of DNA sequences from the *.fasta file. */
    vector<DNASequence> seqList;
    GenomicsFileTypeParser::loadDNASequenceToList(inputfile, seqList);

    /* Step #2. Parse alphabet file to get alphabet for ST. */
    string alphabet = "";
    GenomicsFileTypeParser::loadAlphabet(alphabetfile, alphabet);

    /* Step #3. Construct suffix tree and time it. */
    clock_t start, end;
    double cpu_time_used;
     
    start = clock();

    SuffixTree st = SuffixTree(seqList[0].sequence + '$', alphabet);
    
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    cout << "CPU time usage (seconds): " << cpu_time_used << endl;

    /* Step #4. Report stats. */
   // st.DFSTraversal();
    st.report();

    return 0;
}