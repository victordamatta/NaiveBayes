#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <vector>
#include <map>
#include "trie.h"
#include "bayes.h"
using namespace std;

#define DEBUG 0

int training_size;

/* Transforma uma frase em um vetor de palavras */
vector<string> split (const string& s, char c = ' ') {
    vector<string> v;
    size_t prev = 0;
    for (size_t i = 0; i < s.size (); i++) {
        if (s[i] == c) {
            v.push_back (s.substr (prev, i - prev));
            prev = i + 1;
        }
    }
    v.push_back (s.substr (prev, s.size () - prev));
    return v;
}

int main (int argc, char *argv[]) {
    training_size = 0;
    string line;
    if (argc < 2) {
        cout << "USO: ./llt-main [dados] < [testes]" << endl;
        return 1;
    }
    vector<Trie*> dics;
    map<string, int> meddra_to_it;

    ifstream f1 ("dados/pt_without_marks.asc");
    if (DEBUG) cout << "Initialize classes\n";
    int it = 0;
    while (getline (f1, line)) {
        size_t sep = line.find ("$");
        string medra = line.substr (0, sep);
        string verb = line.substr (sep + 1);
        vector<string> separated = split (verb);
        Trie* t = new Trie (medra);
        meddra_to_it[medra] = it++;
        for (size_t i = 0; i < separated.size(); i++) {
            t->addWord (separated[i], true);
        }
        dics.push_back (t);
    }
    f1.close ();

    /* Lines Version */
    if (DEBUG) cout << "Start training\n";
    ifstream f2 (argv[2]);
    while (getline (f2, line)) {
        training_size++;
        size_t sep = line.find ("$");
        string medra = line.substr (0, sep);
        string verb = line.substr (sep + 1);
        vector<string> separated = split (verb);
        dics[meddra_to_it[medra]]->incrementTimes ();
        for (string s : separated) {
            dics[meddra_to_it[medra]]->addWord (s, false);
        }
    }
    f2.close ();
    if (DEBUG) cout << "End training\n";

    /*
    cout << "Checking vocabulary sizes:\n";
    for (size_t i = 0; i < dics.size (); i++)
    cout << dics[i]->size () << "\n";
    */

    if (DEBUG) cout << "Checking classifier:\n";
    int cs = 0, ics = 0;
    int test_cases = 0;
    while (getline (cin, line)) {
        test_cases++;
        size_t sep = line.find ("$");
        string medra = line.substr (0, sep);
        string verb = line.substr (sep + 1);

        string guess = classify (split (verb), dics);

        if (DEBUG) cout << verb << '\n' << medra << " " << guess << "\n";
        if (medra.compare (guess) == 0) {
            if (DEBUG) cout << "correct\n\n";
            cs++;
        }
        else {
            if (DEBUG) cout << "incorrect\n\n";
            ics++;
        }
    }
    cout << (double) cs / (cs + ics) << "\n";

    return 0;
}
