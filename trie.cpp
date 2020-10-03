#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <ctype.h>
#include "trie.h"
using namespace std;

const string stopwords = "de a o e que do da em para no é com os um na não uma se por dos as mais ao são como foi das à mas ele";

string filter (string s) {
    for (unsigned i = 0; i < s.length (); i++) {
        s[i] = tolower(s[i]);
    }
    return s;
}

Node* Node::findChild (const char c) const {
    for (unsigned i = 0; i < mChildren.size (); i++) {
        Node* tmp = mChildren[i];
        if (tmp->content () == c) return tmp;
    }
    return NULL;
}

void Trie::addWord (string s, bool orig) {
    if (stopwords.find (s) != string::npos) return;
    s = filter(s);
    Node* current = root;
    sz++;

    if (s.length () == 0) {
        current->setWordMarker ();
        if (orig) current->setOriginal ();
        return;
    }

    for (unsigned i = 0; i < s.size (); i++) {
        Node* child = current->findChild (s[i]);
        if (child != NULL) {
            current = child;
        }
        else {
            Node* tmp = (Node*) malloc (sizeof (Node));
            tmp = new Node ();
            tmp->setContent (s[i]);
            current->appendChild (tmp);
            current = tmp;
        }
        if (i == s.size () - 1) {
            current->setWordMarker ();
            if (orig) current->setOriginal ();
            if (current->times () == 0) typ += 1;
            current->incrementTimes ();
        }
    }
}

int Trie::quickSearch (string s, bool orig) const {
    s = filter(s);
    Node* current = root;

    if (s.length () == 0)
        if ((orig && current->original ()) || (!orig && current->wordMarker ()))
            return current->times ();

    for (unsigned i = 0; i < s.size (); i++) {
        Node* child = current->findChild (s[i]);
        if (child != NULL) {
            current = child;
        }
        else {
            return 0;
        }
        if (i == s.size () - 1)
            if ((orig && current->original ()) || (!orig && current->wordMarker ()))
                return current->times ();
    }
    return 0;
}

int Trie::searchWord (string s, bool orig, int cost) const {
    s = filter(s);
    int sz = s.size(), results = 0, maxCost = cost;
    vector<int> current_row(sz + 1);

    for (int i = 0; i <= sz; ++i) current_row[i] = i;

    for (auto letter = root->children().begin(); letter != root->children().end(); letter++) {
        searchRecursive (*letter, s, current_row, maxCost, &results, orig);
    }

    return results;
}

void Trie::searchRecursive (Node* tree, const string& word, const vector<int>& last_row, int max_cost, int * results, const bool orig) const {
    int sz = last_row.size ();
    vector<int> current_row(sz);
    current_row[0] = last_row[0] + 1;

    int insert_or_del, replace;
    for (int i = 1; i < sz; ++i) {
        insert_or_del = min(current_row[i-1] + 1, last_row[i] + 1);
        replace = (word[i-1] == tree->content ()) ? last_row[i-1] : (last_row[i-1] + 1);

        current_row[i] = min(insert_or_del, replace);
    }

    if ((current_row[sz-1] <= max_cost) && tree->wordMarker()) {
        if (orig && tree->original ()) *results += tree->times ();
        else if (!orig) *results += tree->times ();
    }

    if (*min_element(current_row.begin(), current_row.end()) <= max_cost) {
        for (auto it = tree->children().begin (); it != tree->children().end (); it++) {
            searchRecursive(*it, word, current_row, max_cost, results, orig);
        }
    }
}
