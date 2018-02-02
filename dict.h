#ifndef dict_h
#define dict_h

#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <vector>
#include <map>
#include <algorithm>
#include <fstream>
using namespace std;

#define bold "\33[1m"
#define regular "\33[0m"
#define magenta "\33[35m"

class Trie{
    
    class TrieNode{
    public:
        /*
         Each node in `alpha` points to next available character.
         Size of `alpha` can be changed according to the valid characters in a string.
         Here strings in only lowercase are considered to be valid.
         */
        int alpha[26];          // 104
        time_t addedAt;         // 8
        short int searchHits;   // 2
        char addedBy[13];       // 13
        bool isString;          // 1
        
        TrieNode(bool isString, string addedBy, time_t addedAt);
    };
    
    struct dictAttributes{
        int vector_size,
        topResults,
        dictSize,
        root,
        dictEnd;
        char dictName[12];
    } dictAttr;         // 32 bytes
    
    struct typecast{
        union{
            dictAttributes dict;
            char entry_buffer[32];
        };
        typecast() { return; }   // default constructor is delete
    };
    
    int addNode(int rootBlockNo, int index, bool isString = false, string addedBy = "", time_t time = NULL);
    int searchString(string str, int start);
    TrieNode readBlock(int blockNo);
    void writeBlock(TrieNode node, int blockNo);
    int getIndex(char a);
    void findAutoComplete(int blockNo, vector<pair<int, string> > &words, string prefix, int vectorSize, int &insertIndex);
    
public:
    
    Trie(int vector_size, int topResults, int dictSize, int root = 0, int dictEnd = 1, const char* dictName = "myDict");
    void addString(string str, string addedBy);
    void printStringStatus(string str);
    bool printAutoComplete(string prefix);
};

#endif /* dict_h */
