#include "dna_sequence.h"

void string_to_upper(string &s)
{
    for (int i=0; i<(int)s.length(); ++i)
    {
        s[i] = toupper(s[i]);
    }
}

DNASequence::DNASequence()
{
    this->sequence = string("");
    this->id = string("");
    this->description = string("");
}

iostream & operator >> (iostream &lhs, DNASequence &rhs) {
    char line[MAX_LINE] = {'\0'}, *s = NULL;
    string seq, id, description;

    /* Read first line (id and description). */
    lhs.getline(line, MAX_LINE);

    /* ID terminates with the first whitespace in general. However, if multi-word ID, it's wrapped in (). */
    s = strtok(line, ")");
    if (s[0] == '(') {
        /* Take everything to right of '(' */
        rhs.id = string(s).substr(1);
        s++;
    }
    else {
        /* strtok would have returned same string (line), so rerun strtok on whitespace delimiter. */
        s = strtok(line, " ");
        rhs.id = string(s);
    }
    if (DEBUG_MODE) cout << "sequence id: " << rhs.id << endl;

    /* Rest of the line (until newline) is the description. This part is optional, however. */
    s = strtok(NULL, "\n");
    rhs.description = (s != NULL) ? string(s) : "";   // since optional, must check if s is NULL
    if (DEBUG_MODE) cout << "sequence description: " << rhs.description << endl;

    /* The rest of the iostream is the actual DNA -- read until we reach a blank line. */
	while (lhs.getline(line, MAX_LINE)) {
        /* Ensure string is all-upper case. */
        string s(line);
        string_to_upper(s);

        /* Push to sequence attribute. */
        rhs.sequence += s;
	}
    if (DEBUG_MODE) cout << "dna seq after parsing:" << endl << rhs.sequence << endl;

    return lhs;
}