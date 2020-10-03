#include <iostream>
#include <string>
#include <vector>
#include <math.h>
#include "trie.h"
#include "bayes.h"
using namespace std;

extern int training_size;

/* Gera uma classificação para o documento a partir dos dados */
string classify (const vector<string>& document, const vector<Trie*>& data) {
    string cmax;
    double prob = -1000;
    for (size_t it = 0; it < data.size (); it++) {
        double cur = 0;
        if (training_size > 0) cur = (double) data[it]->times () / training_size;
        for (size_t token = 0; token < document.size (); token++) {
            cur += log ((0.5 + data[it]->searchWord (document[token], false, 1)) / (data[it]->types ()/2.0 + data[it]->size ()));
            //cur += log ((0.5 + data[it]->quickSearch (document[token], false)) / (data[it]->types ()/2.0 + data[it]->size ()));
        }
        if (cur > prob) {
            prob = cur;
            cmax = data[it]->getMedra ();
        }
    }
    return cmax;
}
