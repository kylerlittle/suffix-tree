#include "test_helper.h"

string parseBWTFile(string bwtFileName)
{
  fstream bwtFile;
  string bwtString = "";
        
  char line[MAX_LINE] = {'\0'}, *s;

  /* Open fileName for readmode -- must use char * to open. */
  bwtFile.open(bwtFileName.c_str(), std::fstream::in);

  if (bwtFile.is_open()) {
      while (bwtFile.getline(line, MAX_LINE)) {
          bwtString.append(line);
      }
      bwtFile.close();
  } else {
      cout << "parseBWTFile -- bad input file" << endl;
      exit(1);
  }


  return bwtString;
}

string getBWT(string inputFile, string alphabetFile)
{
    vector<DNASequence> seqList;
    GenomicsFileTypeParser::loadDNASequenceToList(inputFile, seqList);
    string alphabet = "";
    GenomicsFileTypeParser::loadAlphabet(alphabetFile, alphabet);
    SuffixTree st = SuffixTree(seqList[0].sequence + '$', alphabet);
    return st.getBWT();
}

unordered_map<int, string> getIDPathLabelPairs(string inputFile, string alphabetFile)
{
    vector<DNASequence> seqList;
    GenomicsFileTypeParser::loadDNASequenceToList(inputFile, seqList);
    string alphabet = "";
    GenomicsFileTypeParser::loadAlphabet(alphabetFile, alphabet);
    SuffixTree st = SuffixTree(seqList[0].sequence + '$', alphabet);
    return st.getIDPathLabelPairs();
}
