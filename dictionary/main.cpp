#include "dict.h"

int main()
{
    Trie myTrie(2, 5, 2);
    bool run = true;
    
    while(run){
        cout<<"> ";
        string input;
        getline(cin, input);
        
        stringstream ss(input);
        string operation="", str="", user="";
        ss>>operation>>str>>user;
        
        if(operation == "add")
            myTrie.addString(str,user);
        else if (operation == "find")
            myTrie.printStringStatus(str);
        else if (operation == "prefix")
            myTrie.printAutoComplete(str);
        else if(operation == "exit")
            run = false;
        else
            cout<<"Invalid"<<endl;
    }
    
    return 0;
}
