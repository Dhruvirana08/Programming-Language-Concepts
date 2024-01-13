//main file for executing PA1

#include "lex.h"
#include <fstream>
#include <iostream>
#include <map>
#include <cctype>
#include <vector>
#include <string>
#include <sstream>
#include <typeinfo>

//using std::map;

using namespace std;

int main(int argc, char* argv[]){
    
    LexItem toks;
    ifstream inClientFile;
    istream* in = &cin;

    //keeping track of token counts and linenums
    int linenum = 0;
    int tok_count = 0;
    int file_c = 0;   //number of files
    
    //map of tokens
    map <string, bool> ident_tok;
    map <float, int> nconst_tok;
  //  map <float, bool> rconst_tok;
    map <string, bool> sconst_tok;
    
    //flags
    bool v_flag = false;
    bool nconsts_flag = false;
    bool sconsts_flag = false;
    bool ident_flag = false;
    // flags: -v, -iconsts, -rconsts , -sconsts , -ident
    
    string filename;
    
    if (argc == 1){
        cerr << "NO SPECIFIED INPUT FILE." << endl;
        return 0;
    }
    
    //look through the input filenames to check which flag it belongs to
    for(int i = 1; i < argc; i++){
        string arg(argv[i]); 
        if(arg[0] != '-'){
            file_c++;
        }
        if(arg == "-v")
            v_flag = true;
        else if(arg == "-nconst")
            nconsts_flag = true;
        else if(arg == "-sconst")
            sconsts_flag = true;
        else if(arg == "-ident")
            ident_flag = true;
        else if(arg[0] == '-'){
            cerr << "UNRECOGNIZED FLAG {" << arg << "}" << endl;
            return 0;
        }
        else{
            inClientFile.open(arg);
            if(file_c > 1){
                cerr << "ONLY ONE FILE NAME IS ALLOWED."<< endl;
                return 0;
            }
            if(inClientFile.is_open() == false){
                cerr << "CANNOT OPEN THE FILE " << arg << endl;
                return 0;
            }
        }
    }
    
    while ((toks = getNextToken(*in, linenum)) != DONE && toks != ERR){
        //cout << toks <<endl;
        tok_count++;
        
        //inserting lexemes into the map
        if(v_flag){
            cout << toks << endl;
        }
        
        if(toks == IDENT){
            ident_tok[toks.GetLexeme()] = true;
        }
        
        //mistake is here 
        if(toks == NIDENT){
            ident_tok[toks.GetLexeme()] = true;
        }
        
        if(toks == SIDENT){
            ident_tok[toks.GetLexeme()] = true;
        }
        
       if(toks == ICONST || toks == RCONST){
           nconst_tok[stod(toks.GetLexeme())]++;
       }
           /* int x;
            stringstream ss;
            ss << toks.GetLexeme();
            ss >> x;
            
            if(!iconst_tok.count(x)){
                iconst_tok[x] = true;
            }
        }
        
       if(toks == RCONST){
            string a = toks.GetLexeme();
            if(a[0] == '.')
                a = "0" + a;
            
            float b;
            stringstream ss;
            ss << a;
            ss >> b;
            
            rconst_tok[b] = true;
        }*/
        
        if(toks == SCONST){
            sconst_tok[toks.GetLexeme()] = true;
        }
    }
    
    if(toks == ERR){
        cout << "Error in line " << toks.GetLinenum() + 1 << " (" << toks.GetLexeme() << ")" << endl;
        return 0;
    }
    /*if(!(ch == '*' && in.peek() == ')')){
                    cout << "Missing a comment end delimiters '*)' at line " << linenum << endl; 
                }*/
    
    
    //print out the final output
    if (linenum != 0) cout << endl;
    cout << "Lines: " << linenum << endl;
    
    if (linenum == 0){
        cout << "Empty File." << endl;
    }
    
    if (linenum > 0){
        cout << "Total Tokens: " << tok_count << endl;
        cout << "Identifiers: " << ident_tok.size() << endl;
        cout << "Numbers: " << nconst_tok.size()<< endl;
        cout << "Strings: " << sconst_tok.size() << endl;
    }
    
    if(ident_flag && ident_tok.size() > 0){
        cout << "IDENTIFIERS:" << endl;
        
        for(auto key = ident_tok.begin(); key != ident_tok.end(); key++){
            cout<< key->first;
            
            if(next(key) != ident_tok.end()){
                cout << ", ";
            }
            //cout << endl;
        }
        cout << endl;
        //return 0;
    }
    
    if(nconsts_flag && nconst_tok.size() > 0){
        cout << "NUMBERS:" << endl;
        
        for(auto key = nconst_tok.begin(); key != nconst_tok.end(); key++){
            cout<< key->first << endl;
        }
   //   cout << endl;
        
    }
    
    if(sconsts_flag && sconst_tok.size() > 0){
        cout << "STRINGS:" << endl;
        
        for(auto key = sconst_tok.begin(); key != sconst_tok.end(); key++){
            cout << "\'" << key -> first << "\'" << endl;
        }
    }
    
}