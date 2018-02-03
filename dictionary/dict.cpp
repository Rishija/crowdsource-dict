#include "dict.h"

Trie::Trie(int vector_size, int topResults, int dictSize, int root, int dictEnd, const char* dictName){
    
    fstream exists;
    exists.open(dictName, ios::in | ios:: binary);
    int size = exists.tellg();
//    ifstream exists(dictName);
    if(size < 0){
        exists.close();
        // Initialize dictionary
        fstream fout;
        fout.open(dictName, ios::out | ios::binary);
        dictAttr.vector_size = vector_size;
        dictAttr.topResults = topResults;
        dictAttr.root = root;
        dictAttr.dictEnd = dictEnd;
        strcpy(dictAttr.dictName, dictName);
        typecast entry;
        entry.dict = this -> dictAttr;
        fout.write((char*)&entry, sizeof(entry));
        
        // write root node
        TrieNode root(false, "admin", NULL);
        fout.write((char*)&root, sizeof(root));
        fout.close();
    }
    else{
        typecast entry;
        exists.read((char*)&entry, sizeof(entry));
        // Get dictionary attributes
        this -> dictAttr = entry.dict;
        exists.close();
    }
}


/**
 Creates an instance of class TrieNode
 Initially all indeces of alpha points to NULL
 Other attributes are specified by the parameters
 */
Trie::TrieNode::TrieNode(bool isString, string addedBy, time_t addedAt){
    
    for(int i=0; i<26; ++i)
        this->alpha[i] = 0;
    this->searchHits = 0;
    this->isString = isString;
    strcpy(this -> addedBy, addedBy.c_str());
    this->addedAt = addedAt;
}


/**
 Reads a TrieNode from the file

 @param blockNo : Block number of TrieNode
 @return Object read
 */
Trie::TrieNode Trie::readBlock(int blockNo){
    
    TrieNode thisNode(0, "", NULL);
    fstream fin;
    fin.open(dictAttr.dictName, ios::in | ios::binary);
    fin.seekg(blockNo*128 + 32);
    fin.read((char*)&thisNode, sizeof(thisNode));
    fin.close();
    return thisNode;
}


/**
 Writes a TrieNode onto the file

 @param node : The Trienode
 @param blockNo : Its block number
 */
void Trie::writeBlock(Trie::TrieNode node, int blockNo){
    
    fstream fout;
    fout.open(dictAttr.dictName, ios::binary | ios::in | ios::out);
    fout.seekp(blockNo*128 + 32);
    fout.write((char*)&node, sizeof(node));
    fout.close();
}


/**
  Adds a new Trienode pointed by root -> alpha[index]

 @param rootBlockNo : Address of TrienNode to which new node is to be added
 @param index : relative position for character to be added
 @param val
     True: if some string ends at this node
     False: otherwise
 @return Block number of new node added
 */
int Trie::addNode(int rootBlockNo, int index, bool val, string addedBy, time_t time){
    
    TrieNode newNode(val, addedBy, time), modify(false, "", NULL);
    modify = readBlock(rootBlockNo);
    writeBlock(newNode, this -> dictAttr.dictEnd);
    modify.alpha[index] = dictAttr.dictEnd;
    writeBlock(modify, rootBlockNo);
    
    // increment dictEnd in file
    fstream fp;
    fp.open(dictAttr.dictName, ios::binary | ios::in | ios::out);
    fp.seekg(12);
    int dictEnd;
    fp.read((char*)&dictEnd, sizeof(int));
    ++dictEnd;
    fp.seekp(12);
    fp.write((char*)&dictEnd, sizeof(int));
    fp.close();
    
    ++(this -> dictAttr.dictEnd);
    return dictAttr.dictEnd - 1;
}


/**
 Returns index of a character
 */
int Trie::getIndex(char a){
    
    return int(a) - 97;
}


/**
 Adds a new string to the dictionary
 
 @param str : string to be added
 */
void Trie::addString(string str, string addedBy){
    
    time_t currentTime = time(0);
    int n = (int)str.length(), currentBlockNo = this -> dictAttr.root;
    TrieNode current = readBlock(currentBlockNo);
    
    for(int i=0; i<n; ++i){
        int index = getIndex(str[i]);
        if(current.alpha[index] == 0){
            
            if(i!=(n-1)){
                currentBlockNo = addNode(currentBlockNo, index);
                current = readBlock(currentBlockNo);
            }
            else{
//                cout<<"Adding new at: "<<put_time(gmtime(&currentTime),"%c %Z");
                addNode(currentBlockNo, index, true, addedBy, currentTime);
            }
        }
        else{
            currentBlockNo = current.alpha[index];
            current = readBlock(current.alpha[index]);
            if(i == (n-1)){
//                cout<<"Modifying old at: "<<put_time(gmtime(&currentTime),"%c %Z");
                current.isString = true;
                strcpy(current.addedBy, addedBy.c_str());
                current.addedAt = currentTime;
                writeBlock(current, currentBlockNo);
            }
        }
    }
}


/**
 Search for a string in the dictionary
 
 @param str : string to be searched
 @param start : block number from where searching is to be done
 @return Terminal node and its block address
 */
int Trie::searchString(string str, int start){
    
    if(str.empty())
        return -1;
    
    int n = (int)str.length();
    TrieNode current = readBlock(start);
    int currentBlockNo = start;
    
    for(int i=0; i<n; ++i){
        int index = getIndex(str[i]);
       currentBlockNo = current.alpha[index];
        if(currentBlockNo){
            current = readBlock(currentBlockNo);
            if(i == (n-1)){
                // incrementing search hits here also increments search hits for prefixes
                return currentBlockNo;
            }
        }
        else
            return -2;
    }
    return -1;    // Xcode compulsion
}


/**
 Prints information about string searched
 */
void Trie::printStringStatus(string str){
    
    int blockNo = searchString(str, dictAttr.root);
    if(blockNo < 0){
        cout<<"Error not yet added to the dictionary"<<endl;
        return;
    }
    TrieNode information = readBlock(blockNo);
    // increment search hits only for errors, not for prefixes
    ++information.searchHits;
    writeBlock(information, blockNo);
    
    if(!information.isString){
        cout<<"Error not yet added to the dictionary"<<endl;
        return;
    }
    
    struct tm *tt = gmtime(&information.addedAt);
    cout<<magenta<<bold<<"Error: "<<regular<<str<<endl
    <<magenta<<bold<<"Added by: "<<regular<<information.addedBy<<endl
    <<magenta<<bold<<"Added at: "<<regular<<put_time(tt, "%c %Z")<<endl
    <<magenta<<bold<<"Search Hits: "<<regular<<information.searchHits<<endl
    <<magenta<<bold<<"Details: "<<regular<<endl;
}


/**
  Recursively finds all the words with given prefix

 @param blockNo : find words from this block number
 @param words : add found words to this vector
 @param prefix : prefix to be searched for
 @param vectorSize : maximum size of vector words
 @param insertIndex : next index where word is to be inserted
 */
void Trie::findAutoComplete(int blockNo, vector<pair<int, string> > &words, string prefix, int vectorSize, int &insertIndex){
    
    if(!blockNo)
        return;
    
    // if vector is full, double its size
    if(insertIndex == (vectorSize - 1)){
        vectorSize = vectorSize<<1;
        words.resize(vectorSize);
    }
    
    TrieNode start(false, "", NULL);
    start = readBlock(blockNo);
    // add to vector if current string is in the dictionary
    if(start.isString){
        pair<int, string> entry;
        entry.first = start.searchHits;
        entry.second = prefix;
        words[++insertIndex] = entry;
    }
    
    // search recursively in subtree
    for(int i=0; i<26; ++i){
        findAutoComplete(start.alpha[i], words, prefix + char(i+97), vectorSize, insertIndex);
    }
}


/**
 Prints all words with given prefix
 
 @return : bool
    false: if no word have given prefix
    true: otherwise
 */
bool Trie::printAutoComplete(string prefix){
    
    vector<pair<int, string> >words(dictAttr.vector_size);
    int insertIndex = -1;
    int currentBlockNo = searchString(prefix, dictAttr.root);
    
    /* no error starts with given prefix
     isString can be null if `prefix` itself is not an error but prefix for some other
     */
    if(currentBlockNo == -2)
        return false;

    findAutoComplete(currentBlockNo, words, prefix, dictAttr.vector_size, insertIndex);
    
    // sort by most searched words
    sort(words.begin(), words.end(), [](const pair<int, string> &a, const pair<int, string> &b) {
        return a.first > b.first;
    });
    
    int printCount = min(dictAttr.topResults, insertIndex + 1);
    for(int i=0; i<printCount; ++i){
        cout<<words[i].second;
        i == (printCount - 1) ? cout<<endl : cout<<", ";
    }
    return true;
}
