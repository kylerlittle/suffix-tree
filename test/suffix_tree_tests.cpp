#include <limits.h>
#include "test_helper.h"
#include "gtest/gtest.h"

namespace {
  TEST(SuffixTreeTest, BWT)
  {
    string str1 = "";
    string str2 = "";

    str1 = parseBWTFile("./data/s1.fas.BWT.txt");
    str2 = getBWT("./src/data/s1.fasta", "./src/data/English_alphabet.txt");
    EXPECT_STRCASEEQ(str1.c_str(), str2.c_str());

    str1 = parseBWTFile("./data/s2.fas.BWT.txt");
    str2 = getBWT("./src/data/s2.fasta", "./src/data/English_alphabet.txt");
    EXPECT_STRCASEEQ(str1.c_str(), str2.c_str());

    str1 = parseBWTFile("./data/Human-BRCA2-cds.fasta.BWT.txt");
    str2 = getBWT("./src/data/Human-BRCA2-cds.fasta", "./src/data/DNA_alphabet.txt");
    EXPECT_STRCASEEQ(str1.c_str(), str2.c_str());
  }

  TEST(SuffixTreeTest, VerifyLeafLabels)
  {
    unordered_map<int, string> expected({
        {0, "MISSISSIPPI$"}, {1, "SSIPPI$"},
        {2, "SSIPPI$"}, {3, "SSIPPI$"},
        {4, "PPI$"}, {5, "PPI$"},
        {6, "PPI$"}, {7, "PPI$"},
        {8, "PI$"}, {9, "I$"},
        {10, "$"}, {11, "$"}
      }), actual;
    actual = getIDPathLabelPairs("./src/data/s2.fasta", "./src/data/English_alphabet.txt");

    unordered_map<int, string>::const_iterator actualfind;

    for (auto item : expected)
    {
      actualfind = actual.find(item.first);
      EXPECT_EQ(item.first, (actualfind == actual.end()) ? 0 : actualfind->first);
      EXPECT_STRCASEEQ(item.second.c_str(), (actualfind == actual.end()) ? "" : actualfind->second.c_str());
    }
  }
}
