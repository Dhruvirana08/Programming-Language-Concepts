#include "lex.h"
#include <cctype>
#include <string>
#include <map>
#include <iostream>
#include <regex>

using namespace std;

static map<Token,string> display_token = {
    //INDENTIFIER
    {IDENT, "IDENT"},
    {NIDENT, "NIDENT"},
    {SIDENT, "SIDENT"},
    
    //Keywords
    {WRITELN, "WRITELN"},
    {IF, "IF"},
    {ELSE, "ELSE"},
    
    //operators, parens, semicolon
    {PLUS, "PLUS"},
    {MINUS, "MINUS"},
    {MULT, "MULT"},
    {DIV, "DIV"},
    {EXPONENT, "EXPONENT"},
    {ASSOP, "ASSOP"},
    {LPAREN, "LPAREN"},
    {RPAREN, "RPAREN"},
    {LBRACES, "LBRACES"},
    {RBRACES, "RBRACES"},
    {NEQ, "NEQ"},
    {NGTHAN, "NGTHAN"},
    {NLTHAN, "NLTHAN"},
    {CAT, "CAT"},
    {SREPEAT, "SREPEAT"},
    {SEQ, "SEQ"},
    {SLTHAN, "SLTHAN"},
    {SGTHAN, "SGTHAN"},
    {COMMA, "COMMA"},
    {SEMICOL, "SEMICOL"},
    
    //an integer, real, and string constant
    {ICONST, "ICONST"},
    {RCONST, "RCONST"},
    {SCONST, "SCONST"},
    
    //error
    {ERR, "ERR"},
    
    //done
    {DONE, "DONE"},
};

//implementing methods
ostream& operator<<(ostream& out, const LexItem& tok){
    
    Token tts = tok.GetToken();
    
    if(tts == IDENT || tts == NIDENT || tts == SIDENT || tts == RCONST || tts == ICONST || tts == SCONST || tts == ERR){
        out << display_token[tts] << "(" << tok.GetLexeme() << ")";
    }
    else{
        out << display_token[tts];
    }
    return out;
}

    //map of just the keywords
static map<string, Token> keywords_map = {

    //Keywords
    {"writeln", WRITELN},
    {"if", IF},
    {"else", ELSE},

};
////////UPDATED UNTIL HERE

LexItem id_or_kw(const string& lexeme, int linenum){

    Token tts = IDENT;
    /*
    for (int x = 0; x < lexeme.length(); x++){
        if(isspace(lexeme[x]))
            continue;
        else if (lexeme[x]=='@') {
            tts = SIDENT; 
            break;
        }
        else if (lexeme[x]=='$') {
            tts = NIDENT;
            break;
        }
        else tts = IDENT;
    }*/
       
    auto key = keywords_map.find(lexeme);
    if(key != keywords_map.end()){
        tts = key -> second;
    }
    else if(lexeme[0] == '@') tts = SIDENT;
    else if (lexeme[0] == '$') tts = NIDENT;
    return LexItem(tts, lexeme, linenum);
}


//bool prevDot = false;
//regex e (([0-9]*)\.([0-9]+));
/*
bool sop = false;
bool comment = false;
*/

LexItem getNextToken(istream& in, int& linenum){
    
    enum TokState { 
        START, INID, INSTRING, ININT, INREAL, INCOMPARE, INCOMMENT
    }
    
    lexstate = START;
    string lexeme;
    char ch;
    char nch;
    bool dot = false;
    
    while(in.get(ch)) {
        switch(lexstate) {
        //case START
        case START:
            if(ch == '\n'){
                linenum++;
            }
            
            //character is a space
            if(isspace(ch)){
                continue;
            }
            
            lexeme = ch;
            
            //identifier
            if(isalpha(ch) || ch == '_') {
                lexstate = INID;
                lexeme = ch;
            }

            else if(ch == '@' && (in.peek() == '_' || isalpha(in.peek()))) {
                lexstate = INID;
                lexeme = ch;
            }

            else if(ch == '$' && (in.peek() == '_' || isalpha(in.peek()))) {
                lexstate = INID;
                lexeme = ch;
            }
            
            //integers
            else if(isdigit(ch)){
                lexstate = ININT;
            }
            
            //string
            else if(ch == '\''){ 
                lexstate = INSTRING;
            } 
            
            //comments
            else if(ch == '#'){
                lexstate = INCOMMENT;
                lexeme += ch;
                in.get(ch);
            }
            
            /*reals
            else if(ch == '.'){
                lexstate = INREAL;
                
            }*/
            
            //it is one of the operators
            else{
                Token tts = ERR;
                switch(ch){
                case '+':
                    tts = PLUS;
                    break;
                case '-':
                    lexeme = ch;
                    nch = tolower(in.peek());

                    /*
                    nch = in.peek();
                    nch = tolower(nch);
                    */

                    if (nch == 'l' || nch == 'g' || nch == 'e'){
                        lexstate = INCOMPARE;
                        continue;
                    }

                    tts = MINUS;
                    break;
                case '*':
                    if (in.peek() == '*'){
                        in.get(ch);
                        tts = SREPEAT;
                        break;
                    }
                    else {
                        tts = MULT;
                        break;
                    }
                case '/':
                    tts = DIV;
                    break;
                case '^':
                    tts = EXPONENT;
                    break; 
                case '=':
                        if (in.peek() == '='){
                        in.get(ch);
                        tts = NEQ;
                        //sop = false;
                        break;
                    }
                    else{
                        tts = ASSOP;
                        //sop = true;
                        break;
                    }
                case '>':
                    tts = NGTHAN;
                    break;
                case '<':
                    tts = NLTHAN;
                    break;
                case '.':
                    tts = CAT;
                    break;
                case '(':
                    tts = LPAREN;
                    break;
                case ')':
                    tts = RPAREN;
                    break;
                case '{':
                    tts = LBRACES;
                    break;
                case '}':
                    tts = RBRACES;
                    break;
                // case '#':  
                    // break;
                case ';':
                    tts = SEMICOL;
                    break;
                case ',':
                    tts = COMMA;
                    break;
                /*case '@':
                    tts = SIDENT;
                    break;
                case '$': 
                    tts = NIDENT;
                    break;*/
                // '-eq','-lt','gt'
                default:
                    return LexItem(tts, lexeme, linenum);
                }
                
                //if none of the above, it must be an error
                return LexItem(tts, lexeme, linenum);
            }
            
            break;
        
        //case ININT
        case ININT:
            if(isdigit(ch)){
                lexeme += ch;
            }
            
            /*else if(ch == '_'){
                if(!isdigit(in.peek())){
                    lexeme += ch;
                    return LexItem(ERR, lexeme, linenum);
                }
                else{
                    lexeme += ch;
                    lexstate = ININT;
                }
            }*/
            /*else if(isalpha(ch)){
                lexstate = INREAL;
                lexeme += ch;
            }*/
            else if(ch == '.'){
                /*if(!isdigit(in.peek())){
                    lexeme += ch;
                    return LexItem(ERR, lexeme, linenum);
                }*/
                /*else if(in.peek() == '.'){
                    lexeme += ch;
                    return LexItem(ERR, lexeme, linenum);
                }*/
                
                //else{
                    //lexeme += ch;
                    //lexstate = INREAL; 
                    
                    
                    //lexeme += ch;
                lexstate = INREAL;  
                in.putback(ch);
                // }
            }  
            else{
                in.putback(ch);
                return LexItem(ICONST, lexeme, linenum);
            }
            break;
            
        //case INID 
        case INID:
            if (isalpha(ch) || isdigit(ch)){
                lexeme += ch;
            }  
            else if(ch == '_'){
                lexeme += ch;
            }
            else{
                in.putback(ch);
                for (int i = 0; i < lexeme.length(); i++){
                    lexeme[i] = (lexeme[i]);
                }
                return id_or_kw(lexeme, linenum);
            }
                            
            break;
            
        //case INSTRING
        case INSTRING:
            
            if(ch == '\n'){
                //lexeme = lexeme.substr(0, lexeme.length()-1);
                return LexItem(ERR, lexeme, linenum);
            }
            lexeme += ch;
            if(ch == '\''){
                lexeme = lexeme.substr(1, lexeme.length()-2);
                return LexItem(SCONST, lexeme, linenum);
            }
            
            break;
            
        //case INREAL - var = dot
        case INREAL:
            if (ch == '.' && isdigit(in.peek()) && !dot){
                dot = true;
                lexeme += ch;
            }

            else if(ch == '.' && !isdigit(in.peek()) && !dot){
                lexeme += ch;
                return LexItem(RCONST, lexeme, linenum);
            }

            else if(isdigit(ch) && dot){
                lexeme += ch;
            }

            else if(ch == '.' && dot){
                lexeme += ch;
                return LexItem(ERR, lexeme, linenum);
            }
            else {
                in.putback(ch);
                return LexItem(RCONST, lexeme, linenum);
                //for RCONST case
            }

            break;
            
        //case INCOMMENT
        case INCOMMENT:
            //in.get(ch);
            if(ch == '\n'){
                linenum++;
                lexstate = START;
            }

            break;

        case INCOMPARE:
             //peek in to check for the ch and nch   
            ch = tolower(ch);
            lexeme+=ch;
            nch = tolower(in.peek());
            // for SGTHAN -gt
            if(ch == 'g' && nch == 't'){
                in.get(ch);
                ch = tolower(ch);
                lexeme += ch;

                return LexItem(SGTHAN, lexeme, linenum);
            }

            // for SEQ -eq
            else if(ch == 'e' && nch == 'q'){
                in.get(ch);
                ch = tolower(ch);
                lexeme += ch;

                return LexItem(SEQ, lexeme, linenum);
            }
            
            // for SLTHAN -lt
            else if(ch == 'l' && nch == 't'){
                in.get(ch);
                ch = tolower(ch);
                lexeme += ch;

                return LexItem(SLTHAN, lexeme, linenum);
            }
            
            else {
                return LexItem(ERR, "bad string compare", linenum);
            }
        }

    } //while loop ends here 


    /*
    if(comment){
         linenum++;
    }
    */
    
    //end of file reached
    
    
    if(in.eof()){
        return LexItem(DONE, "", linenum);
    }
    
    //error 
    return LexItem(ERR, "some error", linenum);
}
