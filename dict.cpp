#include "dict.h"

Trie::Trie(){
    
    ifstream exists(this -> dictAttr.dictName);
    if(!exists.good()){
        // Initialize dictionary
        exists.close();
        fstream fout;
        fout.open(dictAttr.dictName, ios::out | ios::binary);
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
 @return Pointer to the last node of the string. NULL if string not found
 */
Trie::TrieNode Trie::searchString(string str, int start){
    
    if(str.empty()){
        TrieNode empty(false, "", NULL);
        return empty;
    }
    
    int n = (int)str.length();
    TrieNode current = readBlock(start);
    
    for(int i=0; i<n; ++i){
        int index = getIndex(str[i]);
        if(current.alpha[index]){
            if(i == (n-1)){
                int currentBlockNo = current.alpha[index];
                current = readBlock(currentBlockNo);
                ++current.searchHits;
                writeBlock(current, currentBlockNo);
                // ?? increment search
                return current;
            }
            else{
                current = readBlock(current.alpha[index]);
            }
        }
        else{
            TrieNode empty(false, "", NULL);
            return empty;
        }
    }
    // Xcode
    TrieNode empty(false, "", NULL);
    return empty;
}


void Trie::printStringStatus(string str){
    
    TrieNode information = searchString(str, dictAttr.root);
    
    if(!(information.isString))
        cout<<"Error not yet added to the dictionary"<<endl;
    else{
        struct tm *tt = gmtime(&information.addedAt);
        cout<<magenta<<bold<<"Error: "<<regular<<str<<endl
        <<magenta<<bold<<"Added by: "<<regular<<information.addedBy<<endl
        <<magenta<<bold<<"Added at: "<<regular<<put_time(tt, "%c %Z")<<endl
        <<magenta<<bold<<"Search Hits: "<<regular<<information.searchHits<<endl
        <<magenta<<bold<<"Details: "<<regular<<endl;
    }
}
