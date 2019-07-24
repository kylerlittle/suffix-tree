#include "genomics_file_type_parser.h"

namespace GenomicsFileTypeParser
{
    AlgorithmParameters loadAlignAlgoParamsFromFile(string fileName) {
        fstream configFile;
        AlgorithmParameters paramInstance;

        /* Open fileName for readmode -- must use char * to open. */
        configFile.open(fileName.c_str(), std::fstream::in);

        if (configFile.is_open()) {
            configFile >> paramInstance;    // read file directly into paramInstance
            configFile.close();
        } else {
            // throw std::fstream::failure("Failed to open file " + fileName + "\n");
            cout << "loadAlignAlgoParamsFromFile -- bad input file" << endl;
            exit(1);
        }
        
        return paramInstance;
    }

    void loadDNASequenceToList(string fileName, vector<DNASequence>& seqList) {
        fstream fastaFile;
        char sequenceDelimiter = '>', seq[MAX_DNA_SEQ_SIZE];   // each '>' means a new sequence occurs

        /* Open fileName for readmode -- must use char * to open. */
        fastaFile.open(fileName.c_str(), std::fstream::in);

        if (fastaFile.is_open()) {
            /* Get first '>' char. */
            fastaFile.getline(seq, MAX_DNA_SEQ_SIZE, sequenceDelimiter);
            
            /* Get remainder of sequences into seqList. */
            while (fastaFile.getline(seq, MAX_DNA_SEQ_SIZE, sequenceDelimiter)) {
                /* Instantiate new object. */
                DNASequence seqInstance = DNASequence();

                /* Create stringstream instance; fill with the sequence (include metadata). */
                stringstream temp;
                temp << seq;

                /* Utilize overloaded operator to read into DNA Sequence Object. */
                temp >> seqInstance;

                /* Append to seqList. */
                seqList.push_back(seqInstance);
            }
        } else {
            cout << "loadDNASequenceToList -- bad input file" << endl;
            exit(1);
        }

        fastaFile.close();
    }

    void loadAlphabet(string fileName, string &alphabet) {
        fstream alphabetFile;
        
        char line[MAX_LINE] = {'\0'}, *s;

        /* Open fileName for readmode -- must use char * to open. */
        alphabetFile.open(fileName.c_str(), std::fstream::in);

        if (alphabetFile.is_open()) {
            alphabetFile.getline(line, MAX_LINE);
            s = strtok(line, " ");
            while (s) {
                alphabet.append(s);
                s = strtok(NULL, " ");
            }
            alphabetFile.close();
        } else {
            cout << "loadAlphabet -- bad input file" << endl;
            exit(1);
        }
    }
}