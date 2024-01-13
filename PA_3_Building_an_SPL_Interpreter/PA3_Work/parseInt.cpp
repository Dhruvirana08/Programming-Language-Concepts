#include "parserInt.h"

map<string, Value> results;
map<string, Token> SymTable;
map<string, bool> defVar;

queue<Value> *ValQue;
int par = 0;
string var_name;

namespace Parser
{
    bool last_back_pushed = false;
    LexItem pushed_t;

    static LexItem GetNextToken(istream &in, int &line)
    {
        if  (last_back_pushed)
        {
         last_back_pushed = false;
            return pushed_t;
        }
        return getNextToken(in, line);
    }

    static void PushBackToken(LexItem &t)
    {
        if  (last_back_pushed)
        {
            abort();
        }
     last_back_pushed = true;
     pushed_t = t;
    }

}

static int no_of_error = 0;

int ErrCount()
{
    return no_of_error;
}


void ParseError(int line, string msg)
{
    ++no_of_error;
    cout << no_of_error << ". Line # " << line << ": " << msg << endl;
}


bool Prog(istream &in, int &line){
    bool start_status = false;

    start_status = StmtList(in, line);

    if (!start_status)

    {

        ParseError(line, "Missing Left Parenthesis of Writeln Statement");

        return false;
    }

    return start_status;

} 

// StmtList ::= Stmt ;{ Stmt; }
//stmt
bool StmtList(istream &in, int &line){

    bool stmt_status = false;
    LexItem tok_next = Parser::GetNextToken(in, line);
    if (tok_next.GetToken() == DONE)
    {
        cout << "\n(DONE)\n";
        return true;
    }
    if (tok_next.GetLexeme().compare("}") == 0)
    {
       
        Parser::PushBackToken(tok_next);
        return true;
    }
    Parser::PushBackToken(tok_next);
    stmt_status = Stmt(in, line);

    if (!stmt_status)

    {

        ParseError(line, "Missing Statement");

        return false;
    }

    LexItem firstToken = Parser::GetNextToken(in, line);

    if (((firstToken.GetLexeme()).compare(";") != 0))

    {

        ParseError(line, "Missing semicolon3");

        return false;
    }

    stmt_status = StmtList(in, line);

    if (!stmt_status)

    {

        ParseError(line, "Missing Statement");

        return false;
    }

    return stmt_status;

} 

// Stmt ::= AssignStme | WriteLnStmt | IfStmt

bool Stmt(istream &in, int &line){

    bool status_two = false;
    LexItem tok_next = Parser::GetNextToken(in, line);
    if (tok_next.GetToken() == IF)
    {
        status_two = IfStmt(in, line);
       
    }
    else if (tok_next.GetToken() == WRITELN)
        status_two = WritelnStmt(in, line);
    else
    {
        
        Parser::PushBackToken(tok_next);
        status_two = AssignStmt(in, line);
    }

    if (!status_two)

    {

        ParseError(line, "Missing statement");

        return false;
    }

    return status_two;

}

// IfStmt ::= if (Expr) ‘{‘ StmtList ‘}’ [ else ‘{‘ StmtList ‘}’ ]
bool IfStmt(istream &in, int &line){
    LexItem token_if;
    bool status_if_var = false;
    Value retVal;
    status_if_var = Expr(in, line, retVal);
    if (!status_if_var)
    {

        ParseError(line, "Missing expression");

        return false;
    }

    if (retVal.GetType() != VBOOL)
    {
        ParseError(line, "Expression incompitable in if expression");
        return false;
    }
    bool status_statement = false;
    
    if (retVal.GetBool() == true)
    {
        
        token_if = Parser::GetNextToken(in, line);
        if ((token_if.GetLexeme()).compare("{") != 0)

        {

            ParseError(line, "Missing expression");

            return false;
        }
        status_statement = StmtList(in, line);

        if (!status_statement)
        {
            ParseError(line, "Missing Satement List");

            return false;
        }
        token_if = Parser::GetNextToken(in, line);

        if ((token_if.GetLexeme()).compare("}") != 0)
        {
            ParseError(line, "Missing expression");
            return false;
        }
        token_if = Parser::GetNextToken(in, line);
        if (token_if.GetToken() != ELSE)
        {
            Parser::PushBackToken(token_if);
        }
        else
        {
            while (Parser::GetNextToken(in, line).GetLexeme().compare("}") != 0)
                ;
        }
    }
    else
    {
        
        int count = 1;
        token_if = Parser::GetNextToken(in, line);
        if (token_if.GetLexeme() != "{")
        {
           
            ParseError(line, "Parentheses Issue\n");
            return false;
        }
        int count_two = 0;
        while (count > 0)
        {
            token_if = Parser::GetNextToken(in, line);
            
            if (token_if.GetLexeme() == "{")
            {
                count++;
            }
            else if (token_if.GetLexeme() == "}")
            {
                count--;
            }
            fflush(stdout);
            count_two++;
            if (count_two == 20)
            {
                break;
            }
        }

        token_if = Parser::GetNextToken(in, line);

        token_if = Parser::GetNextToken(in, line);
        if ((token_if.GetLexeme()).compare("{") != 0)
        {
            ParseError(line, "Missing expression");
            return false;
        }

        status_statement = StmtList(in, line);
        
        if (!status_statement)
        {
            ParseError(line, "Missing Satement List");
            return false;
        }
        token_if = Parser::GetNextToken(in, line);
        
        if ((token_if.GetLexeme()).compare("}") != 0)

        {

            ParseError(line, "Missing expression");

            return false;
        }
    
    }

    return status_statement;

} 

// AssignStmt ::= Var = Expr

bool AssignStmt(istream &in, int &line){

    bool status_var = false;
    ValQue = new queue<Value>;
    LexItem idtok;
    status_var = Var(in, line, idtok);

    if (!status_var)

    {

        ParseError(line, "Missing Variable");

        return false;
    }

    LexItem token_a;

    token_a = Parser::GetNextToken(in, line);
    if (token_a.GetToken() != ASSOP)

    {

        ParseError(line, "Missing Operator");

        return false;
    }
   
    bool status_expr;
    Value retVal;
    status_expr = ExprList(in, line);
    if (!status_expr)
    {

        ParseError(line, "Missing Expression");

        return false;
    }

    ValType X;
    ValType Z = ValQue->front().GetType();
    string sometime = idtok.GetLexeme();
    if (sometime[0] == '@')
    {
        X = VSTRING;
    }
    else
    {
        X = VINT;
    }
    if (X == VINT && (Z != VREAL && Z != VINT))
    {
        ParseError(line, "Cannot convert to double");
        return false;
    }
    if (Z == VBOOL)
    {
        ParseError(line, "Cannot convert to double or string");
        return false;
    }
    if (X == VSTRING && (Z == VREAL))
    {
        string str = to_string(ValQue->front().GetReal());
        while (str.length() && str.back() == '0')
        {
            str.pop_back();
        }
        if (str.length() && str.back() == '.')
        {
            str.pop_back();
        }
        results[idtok.GetLexeme()] = Value(str);
        SymTable[idtok.GetLexeme()] = idtok.GetToken();

        return status_expr;
    }
    results[idtok.GetLexeme()] = ValQue->front();
    SymTable[idtok.GetLexeme()] = idtok.GetToken();

    return status_expr;

} 

// Var ::= NIDENT | SIDENT

bool Var(istream &in, int &line, LexItem &idtok){

    LexItem token_ident;

    token_ident = Parser::GetNextToken(in, line);
    
    if (token_ident.GetToken() != NIDENT && token_ident.GetToken() != SIDENT)
    {
        
        ParseError(line, "Missing Identifier1");
        return false;
    }
    idtok = token_ident;
    return true;

} 

// WritelnStmt:= WRITELN (ExpreList)
bool WritelnStmt(istream &in, int &line){
    LexItem tok_write;

    ValQue = new queue<Value>;

    tok_write = Parser::GetNextToken(in, line);
    if (tok_write != LPAREN)
    {

        ParseError(line, "Missing Left Parenthesis of Writeln Statement");
        return false;
    }
    bool check_ex = ExprList(in, line);
    if (!check_ex)
    {
        ParseError(line, "Missing expression list after Print");
        while (!(*ValQue).empty())
        {
            ValQue->pop();
        }
        delete ValQue;
        return false;
    }

    while (!(*ValQue).empty())
    {
        Value val_next = (*ValQue).front();
        cout << val_next;
        ValQue->pop();
    }
    cout << endl;

    tok_write = Parser::GetNextToken(in, line);
    if (tok_write != RPAREN)
    {

        ParseError(line, "Missing Right Parenthesis of Writeln Statement");
        return false;
    }
    return true;
} 

// ExprList:= Expr {,Expr}
bool ExprList(istream &in, int &line){
    bool expr_stat = false;
    Value retVal;

    expr_stat = Expr(in, line, retVal);
    if (!expr_stat)
    {
        ParseError(line, "Missing Expression");
        return false;
    }
    ValQue->push(retVal);
    LexItem tok_next = Parser::GetNextToken(in, line);

    if (tok_next == COMMA)
    {
        expr_stat = ExprList(in, line);
    }
    else if (tok_next.GetToken() == ERR)
    {
        ParseError(line, "Unrecognized Input Pattern");
        cout << "(" << tok_next.GetLexeme() << ")" << endl;
        return false;
    }
    else
    {
        Parser::PushBackToken(tok_next);
        return true;
    }
    return expr_stat;
} 

// Expr ::= RelExpr [(-eq|==) RelExpr ]
bool Expr(istream &in, int &line, Value &retVal){
    bool check_realex = false;
    Value rv1, rv2;
    check_realex = RelExpr(in, line, rv1);

    if (!check_realex)
    {
        ParseError(line, "Missing Expression");
        return false;
    }

    LexItem token_next = Parser::GetNextToken(in, line);

    if (token_next == NEQ)
    {
        check_realex = RelExpr(in, line, rv2);

        retVal = (rv1 == rv2);
        if (retVal.IsErr())
        {
            cout << "No suitable comparison\n";
            return false;
        }
    }
    else if (token_next == SEQ)
    {

        check_realex = RelExpr(in, line, rv2);

        retVal = (rv1.SEqual(rv2));
        if (retVal.IsErr())
        {
            cout << "No suitable comparison\n";
            return false;
        }
    }

    else if (token_next.GetToken() == ERR)
    {
        ParseError(line, "Unrecognized Input Pattern");
        cout << "(" << token_next.GetLexeme() << ")" << endl;
        return false;
    }

    else
    {
        Parser::PushBackToken(token_next);
        retVal = rv1;
        return true;
    }
    return check_realex;

} // End of Expr

// RelExpr ::= AddExpr [ ( -lt | -gt | < | > ) AddExpr ]

bool RelExpr(istream &in, int &line, Value &retVal){

    bool check_addex = false;
    Value rv1, rv2;
    check_addex = AddExpr(in, line, rv1);
    retVal = rv1;
    if (retVal.IsErr())
    {
        ParseError(line, "Illegal operand type for the operation.");
    }

    if (!check_addex)

    {

        ParseError(line, "Missing Expression");

        return false;
    }

    LexItem tokRelEx = Parser::GetNextToken(in, line);
    
    if (tokRelEx == NGTHAN || tokRelEx == NLTHAN ||
        tokRelEx == SGTHAN || tokRelEx == SLTHAN)

    {

        check_addex = AddExpr(in, line, rv2);
        if (tokRelEx == NGTHAN)
        {
            retVal = rv1 > rv2;
            if (retVal.IsErr())
            {
                ParseError(line, "Illegal greater operation.");
                return false;
            }
        }
        else if (tokRelEx == SGTHAN)
        {
            retVal = rv1.SGthan(rv2);
            if (retVal.IsErr())
            {
                ParseError(line, "Illegal greater operation.");
                return false;
            }
        }
        else if (tokRelEx == NLTHAN)
        {
            retVal = rv1 < rv2;

            if (retVal.IsErr())
            {
                ParseError(line, "Illegal greater operation.");
                return false;
            }
        }
        else
        {
            retVal = rv1.SLthan(rv2);
            if (retVal.IsErr())
            {
                ParseError(line, "Illegal greater operation.");
                return false;
            }
        }
    }

    else if (tokRelEx.GetToken() == ERR)

    {

        ParseError(line, "Unrecognized Input Pattern");

        cout << "(" << tokRelEx.GetLexeme() << ")" << endl;

        return false;
    }

    else

    {
        
        Parser::PushBackToken(tokRelEx);

        return true;
    }

    return check_addex;

} 

// AddExpr :: MultExpr { ( + | - | .) MultExpr }

bool AddExpr(istream &in, int &line, Value &retVal){

    bool check_multex = false;
    Value rv1;
    check_multex = MultExpr(in, line, rv1);
    retVal = rv1;

    if (rv1.IsErr())
    {
        ParseError(line, "Illegal add operation.");
        return false;
    }

    if (!check_multex)

    {

        ParseError(line, "Missing Expression");

        return false;
    }

    LexItem tokAddEx = Parser::GetNextToken(in, line);

    if (tokAddEx == PLUS || tokAddEx == MINUS || tokAddEx == CAT)

    {
        Value rv2;
        check_multex = MultExpr(in, line, rv2);
        if (tokAddEx == PLUS)
        {
            retVal = rv1 + rv2;
            if (retVal.IsErr())
            {
                ParseError(line, "Illegal addition operation.");
                return false;
            }
        }
        else if (tokAddEx == MINUS)
        {
            retVal = rv1 - rv2;
            if (retVal.IsErr())
            {
                ParseError(line, "Illegal subtraction operation.");
                return false;
            }
        }
        else
        {
            retVal = rv1.Catenate(rv2);
            if (retVal.IsErr())
            {
                ParseError(line, "Illegal concatenation operation.");
                return false;
            }
        }
    }

    else if (tokAddEx.GetToken() == ERR)

    {

        ParseError(line, "Unrecognized Input Pattern");

        cout << "(" << tokAddEx.GetLexeme() << ")" << endl;

        return false;
    }

    else

    {

        Parser::PushBackToken(tokAddEx);

        return true;
    }

    return check_multex;

} 

// MultExpr ::= ExponExpr { ( * | / | **) ExponExpr }

bool MultExpr(istream &in, int &line, Value &retVal){
    bool check_exp_ex = false;
    Value rv1;
    check_exp_ex = ExponExpr(in, line, rv1);
    retVal = rv1;
    if (retVal.IsErr())
    {
        ParseError(line, "1 Illegal operand type for the operation.");
        return false;
    }
    if (!check_exp_ex)
    {
        ParseError(line, "Missing Expression");
        return false;
    }

    LexItem t_exp_ex = Parser::GetNextToken(in, line);

    if (t_exp_ex == MULT || t_exp_ex == DIV || t_exp_ex == SREPEAT)

    {
        Value rv2;
        check_exp_ex = ExponExpr(in, line, rv2);
        if (t_exp_ex == MULT)
        {
            retVal = rv1 * rv2;
            if (retVal.IsErr())
            {
                ParseError(line, "2 Illegal operand type for the operation.");
                return false;
            }
        }
        else if (t_exp_ex == DIV)
        {
            retVal = rv1 / rv2;
            if (retVal.IsErr())
            {
                ParseError(line, "3 Illegal operand type for the operation.");
                return false;
            }
        }
        else
        {
            retVal = rv1.Repeat(rv2);
            if (retVal.IsErr())
            {
                ParseError(line, "4 Illegal operand type for the operation.");
                return false;
            }
        }
    }

    else if (t_exp_ex.GetToken() == ERR)

    {
        ParseError(line, "Unrecognized Input Pattern");
        cout << "(" << t_exp_ex.GetLexeme() << ")" << endl;
        return false;
    }

    else

    {
        Parser::PushBackToken(t_exp_ex);
        return true;
    }
    t_exp_ex = Parser::GetNextToken(in, line);

    while (t_exp_ex == MULT || t_exp_ex == DIV || t_exp_ex == SREPEAT)

    {
        Value rv2;
        check_exp_ex = ExponExpr(in, line, rv2);
        if (t_exp_ex == MULT)
        {
            retVal = retVal * rv2;
            if (retVal.IsErr())
            {
                ParseError(line, "2 Illegal operand type for the operation.");
                return false;
            }
        }
        else if (t_exp_ex == DIV)
        {
            retVal = retVal / rv2;
            if (retVal.IsErr())
            {
                ParseError(line, "3 Illegal operand type for the operation.");
                return false;
            }
        }
        else
        {
            retVal = retVal.Repeat(rv2);
            if (retVal.IsErr())
            {
                ParseError(line, "4 Illegal operand type for the operation.");
                return false;
            }
        }
        t_exp_ex = Parser::GetNextToken(in, line);
    }

    if (t_exp_ex.GetToken() == ERR)

    {
        ParseError(line, "Unrecognized Input Pattern");
        cout << "(" << t_exp_ex.GetLexeme() << ")" << endl;
        return false;
    }

    else

    {
        Parser::PushBackToken(t_exp_ex);
        return true;
    }
    return check_exp_ex;

} 

// ExponExpr ::= UnaryExpr { ^ UnaryExpr }

bool ExponExpr(istream &in, int &line, Value &retVal)
{

    bool check_unary = false;
    Value rv1, rv2;
    check_unary = UnaryExpr(in, line, rv1);
    retVal = rv1;
    if (retVal.IsErr())
    {
        ParseError(line, "5 Illegal operand type for the operation.");
        return false;
    }
    if (!check_unary)
    {
        ParseError(line, "Missing Expression");
        return false;
    }

    LexItem t_unary_ex = Parser::GetNextToken(in, line);

    if (t_unary_ex == EXPONENT)
    {
        check_unary = ExponExpr(in, line, rv2);
        retVal = rv1 ^ rv2;
        if (retVal.IsErr())
        {
            ParseError(line, "6 Illegal operand type for the operation.");
            return false;
        }
    }
    else if (t_unary_ex.GetToken() == ERR)
    {
        ParseError(line, "Unrecognized Input Pattern");
        cout << "(" << t_unary_ex.GetLexeme() << ")" << endl;
        return false;
    }
    else
    {
        Parser::PushBackToken(t_unary_ex);
        return true;
    }

    return check_unary;

} 

// UnaryExpr ::= [( - | + )] PrimaryExp

bool UnaryExpr(istream &in, int &line, Value &retVal){
    LexItem tok_u;
    tok_u = Parser::GetNextToken(in, line);
    
    bool PrimaryEx = false;
    int prefix = 0;

    if ((tok_u.GetLexeme()).compare("+") == 0 || (tok_u.GetLexeme()).compare("-") == 0)
    {
        if ((tok_u.GetLexeme()).compare("-") == 0)
        {
            prefix = -1;
        }
        else
        {
            prefix = 1;
        }
        Value rv1;
        PrimaryEx = PrimaryExpr(in, line, prefix, rv1);
        retVal = rv1;
        if (rv1.IsErr())
        {
            ParseError(line, "Error1");
            return false;
        }
    }
    else
    {

        Parser::PushBackToken(tok_u);
        Value rv1;
        PrimaryEx = PrimaryExpr(in, line, prefix, rv1);
        retVal = rv1;
        if (rv1.IsErr())
        {
            ParseError(line, "Error2");
            return false;
        }
    }

    return PrimaryEx;

} 

// PrimaryExpr ::= IDENT | SIDENT | NIDENT | ICONST | RCONST | SCONST| (Expr)

bool PrimaryExpr(istream &in, int &line, int sign, Value &retVal){
    bool check_ex_last = false;
    LexItem last_tok = Parser::GetNextToken(in, line);

    Value rv1;
    if (last_tok == IDENT || last_tok == NIDENT || last_tok == SIDENT ||
        last_tok == ICONST || last_tok == RCONST || last_tok == SCONST || last_tok == LPAREN ||
        check_ex_last == true)
    {
        check_ex_last = true;
        if (last_tok == NIDENT)
        {
            string sometime = last_tok.GetLexeme();
            if (SymTable.find(sometime) == SymTable.end())
            {
                ParseError(line, "Variable" + sometime + " is not declared");
                retVal.SetType(VERR);
                return false;
            }
            if (sign == -1 || sign == 1)
            {
                auto tok_write = results[sometime].GetType();
                if (tok_write == VREAL)
                {
                    retVal = Value(sign * results[sometime].GetReal());
                }
                else if (tok_write == VINT)
                {
                    retVal = Value(sign * results[sometime].GetInt());
                }
                else
                {
                    ParseError(line, "Illegal Operand Type for prefix Operator");
                    return false;
                }
            }
            else
            {
                retVal = results[sometime];
            }
            return true;
        }
        else if (last_tok == SIDENT)
        {
            string sometime = last_tok.GetLexeme();
            if (SymTable.find(sometime) == SymTable.end())
            {
                ParseError(line, "Variable" + sometime + " is not declared");
                retVal.SetType(VERR);
                return false;
            }
            if (sign != 0)
            {
                ParseError(line, "Illegal Operand Type for prefix Operator");
                return false;
            }
            retVal = results[sometime];
            return true;
        }
        else if (last_tok == ICONST)
        {
            string sometime = last_tok.GetLexeme();

            rv1.SetInt(stoi(sometime));
            retVal = Value(((sign == 0) ? 1 : sign) * stoi(sometime));
            return true;
        }
        else if (last_tok == RCONST)
        {
            string sometime = last_tok.GetLexeme();
            rv1.SetReal(stod(sometime));
            retVal = Value(((sign == 0) ? 1 : sign) * stod(sometime));
            return true;
        }
        else if (last_tok == SCONST)
        {
            string sometime = last_tok.GetLexeme();
            if (sign != 0)
            {
                ParseError(line, "Illegal Operand Type for prefix Operator");
                return false;
            }
            
            retVal = Value(sometime);
            return true;
        }
        else if (last_tok == LPAREN)
        {
            Value retval1;
            Expr(in, line, retVal);
           
            last_tok = Parser::GetNextToken(in, line);
           
            if (last_tok.GetToken() != RPAREN)
            {
                cout << "MISSING RPAREN OF EXPRESSION\n";
                return false;
            }
          
            return true;
        }
    }

    return check_ex_last;

} 