#include "parser.h"
#include <iostream>

using namespace std;

map<string, bool> defVar;
map<string, Token> SymTable;

namespace Parser {
	bool pushed_back = false;
	LexItem	pushed_token;

	static LexItem GetNextToken(istream& in, int& line) {
		if( pushed_back ) {
			pushed_back = false;
			return pushed_token;
		}
		return getNextToken(in, line);
	}

	static void PushBackToken(LexItem & t) {
		if( pushed_back ) {
			abort();
		}
		pushed_back = true;
		pushed_token = t;	
	}

}

// error - DONE

static int error_count = 0;

int ErrCount(){
    return error_count;
}

void ParseError(int line, string msg)
{
	++error_count;
	cout << error_count << ". Line # " << line << ": " << msg << endl;
}

//Prog ::= StmtList
bool Prog(istream& in, int& line){
    bool status;
    status = StmtList(in,line);
    if (!status){
        ParseError(line, "Syntactic error in Program Body");
        return false;
    }
    cout << "(DONE)" << endl;
    return status;
}

//StmtList ::= Stmt ;{ Stmt; }
bool StmtList(istream& in, int& line){
    bool status;
    status = Stmt(in,line);
    if (!status){
        ParseError(line, "Missing Stmt.");
        return false;
    }
    
    LexItem t = Parser::GetNextToken(in,line);
    
    if (t != SEMICOL){
        ParseError(line, "Missing Semicolon in StmtList.");
        return false;
    }
    
    else if (t == SEMICOL){
        t = Parser::GetNextToken(in,line);
        if (t == DONE)
            return true;
        else if (t == RBRACES){
            Parser::PushBackToken(t);
            return true;
        }
        Parser::PushBackToken(t);
        return StmtList(in,line);
    }
    
    else {
        ParseError(line,"Missing Semicolon in stmtlist");
        return false;
    }
    return status;
}

//Stmt ::= AssignStme | WriteLnStmt | IfStmt
bool Stmt(istream& in, int& line){
    LexItem t; 
    t = Parser::GetNextToken(in, line);
    bool status;
    
    switch(t.GetToken()){

        case WRITELN:
            //Parser::PushBackToken(t);
            status = WritelnStmt(in, line);
            break;
        
        case IF:
            Parser::PushBackToken(t);
            status = IfStmt(in, line);
            break;
        
        case NIDENT: 
            Parser::PushBackToken(t);
            /*if (!(defVar.find(t.GetLexeme())->second)){
                ParseError(line,"Undeclared variable");
                ParseError(line, "Missing Left Hand Variable i assignment for nident");
                return false;
            }*/
            status = AssignStmt(in, line);
            break;
            
        case SIDENT: 
            Parser::PushBackToken(t);
            /*if (!(defVar.find(t.GetLexeme())->second)){
                ParseError(line,"Undeclared variable");
                ParseError(line, "Missing Left Hand Variable i assignment for sident");
                return false;
            }*/
            status = AssignStmt(in, line);
            break;

        default:
            Parser::PushBackToken(t);
            return false;
    }

    return status;

}

//WriteLnStmt ::= writeln (ExprList)
bool WritelnStmt(istream& in, int& line){
    LexItem t;
	//cout << "in Writeln Stmt" << endl;
	
	t = Parser::GetNextToken(in, line);
	if( t != LPAREN ) {
		
		ParseError(line, "Missing Left Parenthesis of Writeln Statement");
		return false;
	}
	
	bool ex = ExprList(in, line);
	
	if( !ex ) {
		ParseError(line, "Missing expression list after Print");
		return false;
	}
	
	t = Parser::GetNextToken(in, line);
	if(t != RPAREN ) {
		
		ParseError(line, "Missing Right Parenthesis of Writeln Statement");
		return false;
	}
	//Evaluate: print out the list of expressions values

	return true;
}

//IfStmt ::= if (Expr) '{' StmtList '}' [ else '{' StmtList '}' ]
bool IfStmt(istream& in, int& line){
    LexItem tok = Parser::GetNextToken(in, line);

    bool status = false;
    if(tok.GetToken() != IF){
        ParseError(line,"There is no IF written");
        return false;
    }

    status = Expr(in, line);
    if(!status){
        ParseError(line, "Expr failed");
        return false;
    }
    
    tok = Parser::GetNextToken(in,line);
    if(tok.GetToken() == LBRACES){
        status = StmtList(in,line);
        if(!status){
            ParseError(line,"Incorrect Stmtlist in IF");
            return false;
        }

        tok = Parser::GetNextToken(in,line);
        if (tok.GetToken() != RBRACES){
            ParseError(line, "Missing right brace");
            return false;
        }  
    }
    else{
        ParseError(line, "Missing left brace");
        return false;
    }

    tok = Parser::GetNextToken(in,line);
    if(tok.GetToken() == ELSE){
        tok = Parser::GetNextToken(in,line);
        if(tok.GetToken() == LBRACES){
            status = StmtList(in,line);
            if(!status){
                ParseError(line,"Incorrect Stmtlist in ELSE");
                return false;
            }

            tok = Parser::GetNextToken(in,line);
            if (tok.GetToken() != RBRACES){
                ParseError(line, "Missing right brace");
                return false;
            }  
        }
        else{
            ParseError(line, "Missing left brace");
            return false;
        }
    }

    else if(tok.GetToken() == ERR){
        ParseError(line, "Unrecognized patter ater IF statement.");
        return false;
    }

    else{
        Parser::PushBackToken(tok);
        return true;
    }

    return status;
}

//AssignStmt ::= Var = Expr
bool AssignStmt(istream& in, int& line){
    bool status;
    status = Var(in, line);
    if(!status){
        ParseError(line, "Var failed");
        return false;
    }
    
    LexItem t = Parser::GetNextToken(in, line);
    
    if(t.GetToken() != ASSOP){
        ParseError(line, "Missing Assignment Operator");
        return false;
    }
    
    status = Expr(in, line);
    if(!status){
        ParseError(line, "Missing Expression in Assignment Statment");
        return false;
    }
    
    return true;
}

//Var ::= NIDENT | SIDENT
bool Var(istream& in, int& line){
    LexItem t = Parser::GetNextToken(in, line);
    
    if(t != NIDENT && t != SIDENT){
        ParseError(line, "Not an Assignment");
        return false;
    }
    
    /*if (!(defVar.find(t.GetLexeme())->second)){
        ParseError(line, "Using Undefined Variable for Var");
        return false;
    }*/
    return true;
}

//ExprList ::= Expr { , Expr }
bool ExprList(istream& in, int& line){
    bool status = false;
	//cout << "in ExprList and before calling Expr" << endl;
	status = Expr(in, line);
	if(!status){
		ParseError(line, "Missing Expression");
		return false;
	}
	
	LexItem tok = Parser::GetNextToken(in, line);
	
	if (tok == COMMA) {
		//cout << "before calling ExprList" << endl;
		status = ExprList(in, line);
		//cout << "after calling ExprList" << endl;
	}
	else if(tok.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	else{
		Parser::PushBackToken(tok);
		return true;
	}
	return status;
}

//Expr ::= RelExpr [(-eq|==) RelExpr ]
bool Expr(istream& in, int& line){
   // LexItem tok = Parser:: GetNextToken(in,line);
    LexItem tok;

    bool status;
    status = RelExpr(in, line);
    if(!status){
        ParseError(line, "RelExpr failed");
        return false;
    }

    tok = Parser::GetNextToken(in,line);
    if (tok.GetToken() == SEQ || tok.GetToken() == NEQ){
        status = RelExpr(in,line);
    }
    else if(tok.GetToken() == ERR){
        ParseError(line,"Unrecognized pattern");
        return false;
    }
    else {
        Parser::PushBackToken(tok);
        return true;
    }
    return status;
}

//RelExpr ::= AddExpr [ ( -lt | -gt | < | > ) AddExpr ]
bool RelExpr(istream& in, int& line){
    //LexItem tok = Parser:: GetNextToken(in,line);
    LexItem tok;

    bool status;
    status = AddExpr(in, line);
    if(!status){
        ParseError(line, "AddExpr failed");
        return false;
    }

    tok = Parser::GetNextToken(in,line);
    if (tok.GetToken() == SLTHAN || tok.GetToken() == SGTHAN || tok.GetToken() == NLTHAN || tok.GetToken() == NGTHAN){
        status = AddExpr(in,line);
    }
    else if(tok.GetToken() == ERR){
        ParseError(line,"Unrecognized pattern");
        return false;
    }
    else {
        Parser::PushBackToken(tok);
        return true;
    }
    return status;
}

//AddExpr :: MultExpr { ( + | - | .) MultExpr }
bool AddExpr(istream& in, int& line){
    //LexItem tok = Parser:: GetNextToken(in,line);
    LexItem tok;

    bool status;
    status = MultExpr(in, line);
    if(!status){
        ParseError(line, "MultExpr failed");
        return false;
    }

    tok = Parser::GetNextToken(in,line);
    if (tok.GetToken() == PLUS || tok.GetToken() == MINUS || tok.GetToken() == CAT){
        status = MultExpr(in,line);
    }
    else if(tok.GetToken() == ERR){
        ParseError(line,"Unrecognized pattern");
        return false;
    }
    else {
        Parser::PushBackToken(tok);
        return true;
    }
    return status;
}

//MultExpr ::= ExponExpr { ( * | / | **) ExponExpr }
bool MultExpr(istream& in, int& line){
     //LexItem tok = Parser:: GetNextToken(in,line);
    LexItem tok;

    bool status;
    status = ExponExpr(in, line);
    if(!status){
        ParseError(line, "ExponExpr failed");
        return false;
    }

    tok = Parser::GetNextToken(in,line);
    if (tok.GetToken() == MULT || tok.GetToken() == DIV || tok.GetToken() == SREPEAT){
        status = ExponExpr(in,line);
    }
    else if(tok.GetToken() == ERR){
        ParseError(line,"Unrecognized pattern");
        return false;
    }
    else {
        Parser::PushBackToken(tok);
        return true;
    }
    return status;
}

//ExponExpr ::= UnaryExpr { ^ UnaryExpr }
bool ExponExpr(istream& in, int& line){
    LexItem tok;
     //LexItem tok = Parser:: GetNextToken(in,line);

    bool status;
    status = UnaryExpr(in, line);
    if(!status){
        ParseError(line, "UnaryExpr failed");
        return false;
    }

    tok = Parser::GetNextToken(in,line);
    if (tok.GetToken() == EXPONENT){
        status = UnaryExpr(in,line);
    }
    else if(tok.GetToken() == ERR){
        ParseError(line,"Unrecognized pattern");
        return false;
    }
    else {
        Parser::PushBackToken(tok);
        return true;
    }
    return status;
}

//UnaryExpr ::= [( - | + )] PrimaryExpr
bool UnaryExpr(istream& in, int& line){
    LexItem tok = Parser::GetNextToken(in, line); 

    bool status;

    if(tok == PLUS || tok == MINUS){
        tok = Parser::GetNextToken(in, line); 
    } 
    Parser::PushBackToken(tok);
    status = PrimaryExpr(in, line, 0);
    if(!status){
        ParseError(line, "PrimaryExpr failed");
        return false;
    }

    return true;
}

//PrimaryExpr ::= IDENT | SIDENT | NIDENT | ICONST | RCONST | SCONST | (Expr)
bool PrimaryExpr(istream& in, int& line, int sign){
    LexItem t = Parser::GetNextToken(in, line);
    
    if(t == IDENT || t == NIDENT || t == SIDENT || t == ICONST || t == RCONST || t == SCONST){  
        if(t == IDENT){
            if (!(defVar.find(t.GetLexeme())->second)){
                ParseError(line, "Using Undefined Variable");
                return false;
            }
        }
        return true;
    }
    
    if(t == LPAREN){
        bool status = Expr(in, line); 
        if(!status){
            ParseError(line, "Expr failed");
            return false;
        }
        t = Parser::GetNextToken(in, line); 
        if(t != RPAREN){
            ParseError(line, "Missing ) after expression"); 
            return false;
        }
        return true;
    }
    Parser::PushBackToken(t); 
    return false;
}
