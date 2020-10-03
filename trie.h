#ifndef TRIE_H
#define TRIE_H
using namespace std;

class Node {
private:
    char mContent;
    bool mMarker;
    int mTimes;
    bool mOriginal;
    vector<Node*> mChildren;

public:
    Node () :
        mContent(' '), mMarker(false), mTimes(0), mOriginal(false), mChildren(0)
        {}
    ~Node () {
        for( unsigned i=0; i<mChildren.size(); ++i )
            if( mChildren[i])
            {
                delete mChildren[i];
                mChildren[i] = 0;
            }
        }
    char content () const { return mContent; }
    void setContent (const char c) { mContent = c; }
    bool wordMarker () const { return mMarker; }
    void setWordMarker () { mMarker = true; }
    bool original () const { return mOriginal; }
    void setOriginal () { mOriginal = true; }
    int times () const { return mTimes; }
    void incrementTimes () { mTimes += 1; }
    Node* findChild (const char c) const;
    void appendChild (Node* child) { mChildren.push_back(child); }
    vector<Node*>& children () { return mChildren; }
};

class Trie {
private:
    Node* root;
    int sz;
    int tim;
    int typ;
    string medra;
    void searchRecursive (Node* tree, const string& word, const vector<int>& last_row, int max_cost, int * results, const bool orig) const;

public:
    Trie (const string s) : root(new Node()), sz(0), tim(0), typ(0), medra(s)
    {}
    ~Trie () {
        if (root) {
            free (root);
            root = 0;
        }
    }
    void addWord (const string s, const bool orig);
    int searchWord (const string s, const bool orig, const int cost) const;
    int quickSearch (const string s, const bool orig) const;
    int size () const { return sz; }
    int times () const { return tim; }
    void incrementTimes () { tim += 1; }
    int types () const { return typ; }
    string getMedra () const { return medra; }
};

#endif
