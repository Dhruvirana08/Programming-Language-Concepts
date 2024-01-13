#ifndef VALUE_H
#define VALUE_H

#include <iostream>
#include <string>
#include <queue>
#include <map>
#include <iomanip>
#include <stdexcept>
#include <cmath>
#include <sstream>

using namespace std;

enum ValType { VINT, VREAL, VSTRING, VBOOL, VERR };

class Value {
    ValType	T;
    bool    Btemp;
    int 	Itemp;
	double   Rtemp;
    string	Stemp;
    
       
public:
    Value() : T(VERR), Btemp(false), Itemp(0), Rtemp(0.0), Stemp("") {}
    Value(bool vb) : T(VBOOL), Btemp(vb), Itemp(0), Rtemp(0.0), Stemp("") {}
    Value(double vr) : T(VREAL), Btemp(false), Itemp(0), Rtemp(vr), Stemp("") {}
    Value(string vs) : T(VSTRING), Btemp(false), Itemp(0), Rtemp(0.0), Stemp(vs) {}
    Value(int vi) : T(VINT), Btemp(false), Itemp(vi), Rtemp(0.0), Stemp("") {}
    
    ValType GetType() const { return T; }
    bool IsErr() const { return T == VERR; }
     bool IsString() const { return T == VSTRING; }
    bool IsReal() const {return T == VREAL;}
    bool IsBool() const {return T == VBOOL;}
    bool IsInt() const { return T == VINT; }
    
    int GetInt() const { if( IsInt() ) return Itemp; throw "RUNTIME ERROR: Value not an integer"; }
    
    string GetString() const { if( IsString() ) return Stemp; throw "RUNTIME ERROR: Value not a string"; }
    
    double GetReal() const { if( IsReal() ) return Rtemp; throw "RUNTIME ERROR: Value not an integer"; }
    
    bool GetBool() const {if(IsBool()) return Btemp; throw "RUNTIME ERROR: Value not a boolean";}
    
    void SetType(ValType type)
    {
    	T = type;
	}
	
	void SetInt(int val)
    {
    	Itemp = val;
	}
	
	void SetReal(double val)
    {
    	Rtemp = val;
	}
	
	void SetString(string val)
    {
    	Stemp = val;
	}
	
	void SetBool(bool val)
    {
    	Btemp = val;
	}
	
    bool valid (const string s) const{
        for (auto x : s){
            if ((x > '9' || x < '0') && x != '.'){
                return 0;
            }
        }
        return 1;
    }
       
    // numeric overloaded add op to this
    Value operator+(const Value& op) const{
        const Value ans;
        if (T == VERR || op.GetType() == VERR)
        {
            return ans;
        }
        if (T == VBOOL || op.GetType() == VBOOL)
        {
            fflush(stdout);
            return ans;
        }
        if (T == VSTRING)
        {
            if (!valid(GetString()))
            {
                cout << "Invalid conversion from string to double.\n";
                const Value ans;
                return ans;
            }
            if (op.GetType() == VSTRING)
            {
                if (!valid(op.GetString()))
                {
                    cout << "Invalid conversion from string to double.\n";
                    const Value ans;
                    return ans;
                }
                string c_one = GetString();
                string c_two = op.GetString();
                if (c_one.find('.') != string::npos || c_two.find('.') != string::npos)
                {
                    double num_one = stod(c_one);
                    double num_two = stod(c_two);
                    return num_one + num_two;
                }
                else
                {
                    int num_one = stoi(c_one);
                    int num_two = stoi(c_two);
                    return num_one + num_two;
                }
            }
            else if (op.GetType() == VINT)
            {
                string c_one = GetString();
                if (c_one.find('.') == string::npos)
                {
                    int num_one = stoi(c_one);
                    int num_two = op.GetInt();
                    return num_one + num_two;
                }
                else
                {
                    double num_one = stod(c_one);
                    double num_two = op.GetInt();
                    return num_one + num_two;
                }
            }
            else
            {
                string c_one = GetString();
                double num_one = stod(c_one);
                double num_two = op.GetReal();
                return num_one + num_two;
            }
        }
        else if (T == VINT)
        {
            int num_one = GetInt();
            if (op.GetType() == VSTRING)
            {
                if (!valid(op.GetString()))
                {
                    cout << "Invalid conversion from string to double.\n";
                    const Value ans;
                    return ans;
                }
                string s = op.GetString();
                if (s.find('.') == string::npos)
                {
                    int num_two = stoi(s);
                    return num_one + num_two;
                }
                else
                {
                    double num_two = stod(s);
                    return num_one + num_two;
                }
            }
            else if (op.GetType() == VINT)
            {
                int num_two = op.GetInt();
                return num_one + num_two;
            }
            else
            {
                double num_two = op.GetReal();
                return num_one + num_two;
            }
        }
        else
        {
            double num_one = GetReal();
            if (op.GetType() == VSTRING)
            {
                if (!valid(op.GetString()))
                {
                    cout << "Invalid conversion from string to double.\n";
                    const Value ans;
                    return ans;
                }
                string s = op.GetString();
                if (s.find('.') == string::npos)
                {
                    int num_two = stoi(s);
                    return num_one + num_two;
                }
                else
                {
                    double num_two = stod(s);
                    return num_one + num_two;
                }
            }
            else if (op.GetType() == VINT)
            {
                int num_two = op.GetInt();
                return num_one + num_two;
            }
            else
            {
                double num_two = op.GetReal();
                return num_one + num_two;
            }
        }
    }
    
    // numeric overloaded subtract op from this
    Value operator-(const Value& op) const{
        if (T == VERR || op.GetType() == VERR)
        {
            const Value ans;
            return ans;
        }
        if (T == VSTRING)
        {
            if (!valid(GetString()))
            {
                cout << "Invalid conversion from string to double.\n";
                const Value ans;
                return ans;
            }
            if (op.GetType() == VSTRING)
            {
                if (!valid(op.GetString()))
                {
                    cout << "Invalid conversion from string to double.\n";
                    const Value ans;
                    return ans;
                }
                string c_one = GetString();
                string c_two = op.GetString();
                if (c_one.find('.') != string::npos || c_two.find('.') != string::npos)
                {
                    double num_one = stod(c_one);
                    double num_two = stod(c_two);
                    return num_one - num_two;
                }
                else
                {
                    int num_one = stoi(c_one);
                    int num_two = stoi(c_two);
                    return num_one - num_two;
                }
            }
            else if (op.GetType() == VINT)
            {
                string c_one = GetString();
                if (c_one.find('.') == string::npos)
                {
                    int num_one = stoi(c_one);
                    int num_two = op.GetInt();
                    return num_one - num_two;
                }
                else
                {
                    double num_one = stod(c_one);
                    double num_two = op.GetInt();
                    return num_one - num_two;
                }
            }
            else
            {
                string c_one = GetString();
                double num_one = stod(c_one);
                double num_two = op.GetReal();
                return num_one - num_two;
            }
        }
        else if (T == VINT)
        {
            int num_one = GetInt();
            if (op.GetType() == VSTRING)
            {
                if (!valid(op.GetString()))
                {
                    cout << "Invalid conversion from string to double.\n";
                    const Value ans;
                    return ans;
                }
                string str = op.GetString();
                if (str.find('.') == string::npos)
                {
                    int num_two = stoi(str);
                    return num_one - num_two;
                }
                else
                {
                    double num_two = stod(str);
                    return num_one - num_two;
                }
            }
            else if (op.GetType() == VINT)
            {
                int num_two = op.GetInt();
                return num_one - num_two;
            }
            else
            {
                double num_two = op.GetReal();
                return num_one - num_two;
            }
        }
        else
        {
            double num_one = GetReal();
            if (op.GetType() == VSTRING)
            {
                if (!valid(op.GetString()))
                {
                    cout << "Invalid conversion from string to double.\n";
                    const Value ans;
                    return ans;
                }
                string str = op.GetString();
                if (str.find('.') == string::npos)
                {
                    int num_two = stoi(str);
                    return num_one - num_two;
                }
                else
                {
                    double num_two = stod(str);
                    return num_one - num_two;
                }
            }
            else if (op.GetType() == VINT)
            {
                int num_two = op.GetInt();
                return num_one - num_two;
            }
            else
            {
                double num_two = op.GetReal();
                return num_one - num_two;
            }
        }
    }
    
    // numeric overloaded multiply this by op
    Value operator*(const Value& op) const{
        if (T == VERR || op.GetType() == VERR)
        {
            const Value ans;
            return ans;
        }
        if (T == VSTRING)
        {
            if (!valid(GetString()))
            {
                cout << "Invalid conversion from string to double.\n";
                const Value ans;
                return ans;
            }
            if (op.GetType() == VSTRING)
            {
                if (!valid(op.GetString()))
                {
                    cout << "Invalid conversion from string to double.\n";
                    const Value ans;
                    return ans;
                }
                string c_one = GetString();
                string c_two = op.GetString();
                if (c_one.find('.') != string::npos || c_two.find('.') != string::npos)
                {
                    double num_one = stod(c_one);
                    double num_two = stod(c_two);
                    return num_one * num_two;
                }
                else
                {
                    int num_one = stoi(c_one);
                    int num_two = stoi(c_two);
                    return num_one * num_two;
                }
            }
            else if (op.GetType() == VINT)
            {
                string c_one = GetString();
                if (c_one.find('.') == string::npos)
                {
                    int num_one = stoi(c_one);
                    int num_two = op.GetInt();
                    return num_one * num_two;
                }
                else
                {
                    double num_one = stod(c_one);
                    double num_two = op.GetInt();
                    return num_one * num_two;
                }
            }
            else
            {
                string c_one = GetString();
                double num_one = stod(c_one);
                double num_two = op.GetReal();
                return num_one * num_two;
            }
        }
        else if (T == VINT)
        {
            int num_one = GetInt();
            if (op.GetType() == VSTRING)
            {
                if (!valid(op.GetString()))
                {
                    cout << "Invalid conversion from string to double.\n";
                    const Value ans;
                    return ans;
                }
                string str = op.GetString();
                if (str.find('.') == string::npos)
                {
                    int num_two = stoi(str);
                    return num_one * num_two;
                }
                else
                {
                    double num_two = stod(str);
                    return num_one * num_two;
                }
            }
            else if (op.GetType() == VINT)
            {
                int num_two = op.GetInt();
                return num_one * num_two;
            }
            else
            {
                double num_two = op.GetReal();
                return num_one * num_two;
            }
        }
        else
        {
            double num_one = GetReal();
            if (op.GetType() == VSTRING)
            {
                if (!valid(op.GetString()))
                {
                    cout << "Invalid conversion from string to double.\n";
                    const Value ans;
                    return ans;
                }
                string str = op.GetString();
                if (str.find('.') == string::npos)
                {
                    int num_two = stoi(str);
                    return num_one * num_two;
                }
                else
                {
                    double num_two = stod(str);
                    return num_one * num_two;
                }
            }
            else if (op.GetType() == VINT)
            {
                int num_two = op.GetInt();
                return num_one * num_two;
            }
            else
            {
                double num_two = op.GetReal();
                return num_one * num_two;
            }
        }
    }
    
    // numeric overloaded divide this by op
    Value operator/(const Value& op) const{
        if (T == VERR || op.GetType() == VERR)
        {
            const Value ans;
            return ans;
        }
        if (T == VSTRING)
        {
            if (!valid(GetString()))
            {
                cout << "Invalid conversion from string to double.\n";
                const Value ans;
                return ans;
            }
            if (op.GetType() == VSTRING)
            {
                if (!valid(op.GetString()))
                {
                    cout << "Invalid conversion from string to double.\n";
                    const Value ans;
                    return ans;
                }
                string c_one = GetString();
                string c_two = op.GetString();
                if (c_one.find('.') != string::npos || c_two.find('.') != string::npos)
                {
                    double num_one = stod(c_one);
                    double num_two = stod(c_two);
                    if (num_two == 0)
                    {
                        cout << "Run-Time Error-Illegal Division by Zero\n";
                        const Value ans;
                        return ans;
                    }
                    return num_one / num_two;
                }
                else
                {
                    int num_one = stoi(c_one);
                    int num_two = stoi(c_two);
                    if (num_two == 0)
                    {
                        cout << "Run-Time Error-Illegal Division by Zero\n";
                        const Value ans;
                        return ans;
                    }
                    return num_one / num_two;
                }
            }
            else if (op.GetType() == VINT)
            {
                string c_one = GetString();
                if (c_one.find('.') == string::npos)
                {
                    int num_one = stoi(c_one);
                    int num_two = op.GetInt();
                    if (num_two == 0)
                    {
                        cout << "Run-Time Error-Illegal Division by Zero\n";
                        const Value ans;
                        return ans;
                    }
                    return num_one / num_two;
                }
                else
                {
                    double num_one = stod(c_one);
                    double num_two = op.GetInt();
                    if (num_two == 0)
                    {
                        cout << "Run-Time Error-Illegal Division by Zero\n";
                        const Value ans;
                        return ans;
                    }
                    return num_one / num_two;
                }
            }
            else
            {
                string c_one = GetString();
                double num_one = stod(c_one);
                double num_two = op.GetReal();
                if (num_two == 0)
                {
                    cout << "Run-Time Error-Illegal Division by Zero\n";
                    const Value ans;
                    return ans;
                }
                return num_one / num_two;
            }
        }
        else if (T == VINT)
        {
            int num_one = GetInt();
            if (op.GetType() == VSTRING)
            {
                if (!valid(op.GetString()))
                {
                    cout << "Invalid conversion from string to double.\n";
                    const Value ans;
                    return ans;
                }
                string str = op.GetString();
                if (str.find('.') == string::npos)
                {
                    int num_two = stoi(str);
                    if (num_two == 0)
                    {
                        cout << "Run-Time Error-Illegal Division by Zero\n";
                        const Value ans;
                        return ans;
                    }
                    return num_one / num_two;
                    if (num_two == 0)
                    {
                        cout << "Run-Time Error-Illegal Division by Zero\n";
                        const Value ans;
                        return ans;
                    }
                }
                else
                {
                    double num_two = stod(str);
                    if (num_two == 0)
                    {
                        cout << "Run-Time Error-Illegal Division by Zero\n";
                        const Value ans;
                        return ans;
                    }
                    return num_one / num_two;
                    if (num_two == 0)
                    {
                        cout << "Run-Time Error-Illegal Division by Zero\n";
                        const Value ans;
                        return ans;
                    }
                }
            }
            else if (op.GetType() == VINT)
            {
                int num_two = op.GetInt();
                if (num_two == 0)
                {
                    cout << "Run-Time Error-Illegal Division by Zero\n";
                    const Value ans;
                    return ans;
                }
                return num_one / num_two;
                if (num_two == 0)
                {
                    cout << "Run-Time Error-Illegal Division by Zero\n";
                    const Value ans;
                    return ans;
                }
            }
            else
            {
                double num_two = op.GetReal();
                if (num_two == 0)
                {
                    cout << "Run-Time Error-Illegal Division by Zero\n";
                    const Value ans;
                    return ans;
                }
                return num_one / num_two;
                if (num_two == 0)
                {
                    cout << "Run-Time Error-Illegal Division by Zero\n";
                    const Value ans;
                    return ans;
                }
            }
        }
        else
        {
            double num_one = GetReal();
            if (op.GetType() == VSTRING)
            {
                if (!valid(op.GetString()))
                {
                    cout << "Invalid conversion from string to double.\n";
                    const Value ans;
                    return ans;
                }
                string str = op.GetString();
                if (str.find('.') == string::npos)
                {
                    int num_two = stoi(str);
                    if (num_two == 0)
                    {
                        cout << "Run-Time Error-Illegal Division by Zero\n";
                        const Value ans;
                        return ans;
                    }
                    return num_one / num_two;
                }
                else
                {
                    double num_two = stod(str);
                    if (num_two == 0)
                    {
                        cout << "Run-Time Error-Illegal Division by Zero\n";
                        const Value ans;
                        return ans;
                    }
                    return num_one / num_two;
                }
            }
            else if (op.GetType() == VINT)
            {
                int num_two = op.GetInt();
                if (num_two == 0)
                {
                    cout << "Run-Time Error-Illegal Division by Zero\n";
                    const Value ans;
                    return ans;
                }
                return num_one / num_two;
            }
            else
            {
                double num_two = op.GetReal();
                if (num_two == 0)
                {
                    cout << "Run-Time Error-Illegal Division by Zero\n";
                    const Value ans;
                    return ans;
                }
                return num_one / num_two;
            }
        }
        Value ans;
        return ans;
    }
    
    
    Value operator==(const Value& op) const{
        if (T == VERR || op.T == VERR)
        {
            const Value ans;
            return ans;
        }
        if (T == VSTRING)
        {
            if (op.GetType() == VSTRING)
            {
                string c_one = GetString();
                string c_two = op.GetString();
                if (!valid(c_one) || !valid(c_two))
                {
                    cout << "Invalid conversion from string to double.\n";
                    const Value ans;
                    return ans;
                }
                return c_one == c_two;
            }
            else if (op.GetType() == VINT)
            {
                string c_one = GetString();
                if (!valid(c_one))
                {
                    cout << "Invalid conversion from string to double.\n";
                    const Value ans;
                    return ans;
                }
                if (c_one.find('.') == string::npos)
                {
                    int num_one = stoi(c_one);
                    int num_two = op.GetInt();
                    return num_one == num_two;
                }
                else
                {
                    double num_one = stod(c_one);
                    double num_two = op.GetInt();
                    return num_one == num_two;
                }
            }
            else
            {
                string c_one = GetString();
                if (!valid(c_one))
                {
                    cout << "Invalid conversion from string to double.\n";
                    const Value ans;
                    return ans;
                }
                double num_one = stod(c_one);
                double num_two = op.GetReal();
                return num_one == num_two;
            }
        }
        else if (T == VINT)
        {
            int num_one = GetInt();
            if (op.GetType() == VSTRING)
            {
                if (!valid(op.GetString()))
                {
                    cout << "Invalid conversion from string to double.\n";
                    const Value ans;
                    return ans;
                }
                string str = op.GetString();
                if (str.find('.') == string::npos)
                {
                    int num_two = stoi(str);
                    return num_one == num_two;
                }
                else
                {
                    double num_two = stod(str);
                    return num_one == num_two;
                }
            }
            else if (op.GetType() == VINT)
            {
                int num_two = op.GetInt();
                return num_one == num_two;
            }
            else
            {
                double num_two = op.GetReal();
                return num_one == num_two;
            }
        }
        else
        {
            double num_one = GetReal();
            if (op.GetType() == VSTRING)
            {
                if (!valid(op.GetString()))
                {
                    cout << "Invalid conversion from string to double.\n";
                    const Value ans;
                    return ans;
                }
                string str = op.GetString();
                if (str.find('.') == string::npos)
                {
                    int num_two = stoi(str);
                    return num_one == num_two;
                }
                else
                {
                    double num_two = stod(str);
                    return num_one == num_two;
                }
            }
            else if (op.GetType() == VINT)
            {
                int num_two = op.GetInt();
                return num_one == num_two;
            }
            else
            {
                double num_two = op.GetReal();
                return num_one == num_two;
            }
        }
    }
	//numeric overloaded greater than operator of this with op
	Value operator>(const Value& op) const{
        if (T == VERR || op.T == VERR)
        {
            const Value ans;
            return ans;
        }
        if (T == VSTRING)
        {
            if (op.GetType() == VSTRING)
            {
                string c_one = GetString();
                string c_two = op.GetString();
                if (!valid(c_one) || !valid(c_two))
                {
                    cout << "Invalid conversion from string to double.\n";
                    const Value ans;
                    return ans;
                }
                if (c_one.find('.') != string::npos || c_two.find('.') == string::npos)
                {
                    double num_one = stod(c_one);
                    double num_two = stod(c_two);
                    return num_one > num_two;
                }
                else
                {
                    int num_one = stoi(c_one);
                    int num_two = stoi(c_two);
                    return num_one > num_two;
                }
            }
            else if (op.GetType() == VINT)
            {
                string c_one = GetString();
                if (!valid(c_one))
                {
                    cout << "Invalid conversion from string to double.\n";
                    const Value ans;
                    return ans;
                }
                if (c_one.find('.') == string::npos)
                {
                    int num_one = stoi(c_one);
                    int num_two = op.GetInt();
                    return num_one > num_two;
                }
                else
                {
                    double num_one = stod(c_one);
                    double num_two = op.GetInt();
                    return num_one > num_two;
                }
            }
            else
            {
                string c_one = GetString();
                if (!valid(c_one))
                {
                    cout << "Invalid conversion from string to double.\n";
                    const Value ans;
                    return ans;
                }
                double num_one = stod(c_one);
                double num_two = op.GetReal();
                return num_one > num_two;
            }
        }
        else if (T == VINT)
        {
            int num_one = GetInt();
            if (op.GetType() == VSTRING)
            {
                if (!valid(op.GetString()))
                {
                    cout << "Invalid conversion from string to double.\n";
                    const Value ans;
                    return ans;
                }
                string str = op.GetString();
                if (str.find('.') == string::npos)
                {
                    int num_two = stoi(str);
                    return num_one > num_two;
                }
                else
                {
                    double num_two = stod(str);
                    return num_one > num_two;
                }
            }
            else if (op.GetType() == VINT)
            {
                int num_two = op.GetInt();
                return num_one > num_two;
            }
            else
            {
                double num_two = op.GetReal();
                return num_one > num_two;
            }
        }
        else
        {
            double num_one = GetReal();
            if (op.GetType() == VSTRING)
            {
                if (!valid(op.GetString()))
                {
                    cout << "Invalid conversion from string to double.\n";
                    const Value ans;
                    return ans;
                }
                string str = op.GetString();
                if (str.find('.') == string::npos)
                {
                    int num_two = stoi(str);
                    return num_one > num_two;
                }
                else
                {
                    double num_two = stod(str);
                    return num_one > num_two;
                }
            }
            else if (op.GetType() == VINT)
            {
                int num_two = op.GetInt();
                return num_one > num_two;
            }
            else
            {
                double num_two = op.GetReal();
                return num_one > num_two;
            }
        }
    }
    
	//numeric overloaded less than operator of this with op
	Value operator<(const Value& op) const{
        {
            if (T == VERR || op.T == VERR)
            {
                const Value ans;
                return ans;
            }
            if (T == VSTRING)
            {
                if (op.GetType() == VSTRING)
                {
                    string c_one = GetString();
                    string c_two = op.GetString();
                    if (!valid(c_one) || !valid(c_two))
                    {
                        cout << "Invalid conversion from string to double.\n";
                        const Value ans;
                        return ans;
                    }
                    if (c_one.find('.') != string::npos || c_two.find('.') == string::npos)
                    {
                        double num_one = stod(c_one);
                        double num_two = stod(c_two);
                        return num_one < num_two;
                    }
                    else
                    {
                        int num_one = stoi(c_one);
                        int num_two = stoi(c_two);
                        return num_one < num_two;
                    }
                }
                else if (op.GetType() == VINT)
                {
                    string c_one = GetString();
                    if (!valid(c_one))
                    {
                        cout << "Invalid conversion from string to double.\n";
                        const Value ans;
                        return ans;
                    }
                    if (c_one.find('.') == string::npos)
                    {
                        int num_one = stoi(c_one);
                        int num_two = op.GetInt();
                        return num_one < num_two;
                    }
                    else
                    {
                        double num_one = stod(c_one);
                        double num_two = op.GetInt();
                        return num_one < num_two;
                    }
                }
                else
                {
                    string c_one = GetString();
                    if (!valid(c_one))
                    {
                        cout << "Invalid conversion from string to double.\n";
                        const Value ans;
                        return ans;
                    }
                    double num_one = stod(c_one);
                    double num_two = op.GetReal();
                    return num_one < num_two;
                }
            }
            else if (T == VINT)
            {
                int num_one = GetInt();
                if (op.GetType() == VSTRING)
                {
                    if (!valid(op.GetString()))
                    {
                        cout << "Invalid conversion from string to double.\n";
                        const Value ans;
                        return ans;
                    }
                    string str = op.GetString();
                    if (str.find('.') == string::npos)
                    {
                        int num_two = stoi(str);
                        return num_one < num_two;
                    }
                    else
                    {
                        double num_two = stod(str);
                        return num_one < num_two;
                    }
                }
                else if (op.GetType() == VINT)
                {
                    int num_two = op.GetInt();
                    return num_one < num_two;
                }
                else
                {
                    double num_two = op.GetReal();
                    return num_one < num_two;
                }
            }
            else
            {
                double num_one = GetReal();
                if (op.GetType() == VSTRING)
                {
                    if (!valid(op.GetString()))
                    {
                        cout << "Invalid conversion from string to double.\n";
                        const Value ans;
                        return ans;
                    }
                    string str = op.GetString();
                    if (str.find('.') == string::npos)
                    {
                        int num_two = stoi(str);
                        return num_one < num_two;
                    }
                    else
                    {
                        double num_two = stod(str);
                        return num_one < num_two;
                    }
                }
                else if (op.GetType() == VINT)
                {
                    int num_two = op.GetInt();
                    return num_one < num_two;
                }
                else
                {
                    double num_two = op.GetReal();
                    return num_one < num_two;
                }
            }
        }
    }
	
	//Numeric exponentiation operation this raised to the power of op 
	Value operator^(const Value& oper) const{
        const Value ans;
        if (GetType() == VERR || oper.GetType() == VERR)
        {
            return ans;
        }
        if (GetType() == VSTRING || oper.GetType() == VSTRING)
        {
            return ans;
        }
        if (T == VREAL)
        {
            double num_one = GetReal();
            if (oper.GetType() == VREAL)
            {
                double num_two = oper.GetReal();
                return pow(double(num_one), int(num_two));
            }
            else
            {
                int num_two = oper.GetInt();
                return pow(double(num_one), int(num_two));
            }
        }
        else
        {
            int num_one = GetInt();
            if (oper.GetType() == VREAL)
            {
                double num_two = oper.GetReal();
                return pow(double(num_one), (num_two));
            }
            else
            {
                int num_two = oper.GetInt();
                return (pow(double(num_one), (num_two)));
            }
        }
    }

// start here now--------------------------------------------------------------------------------------------------------------------------------------------------------
    // string concatenation operation of this with op
    Value Catenate(const Value &oper) const
    {
        const Value ans;
        if (oper.GetType() == VERR || T == VERR)
        {
            return ans;
        }
        ValType val_type_one = GetType();
        ValType val_type_two = oper.GetType();
        string c_two;
        if (val_type_two == VREAL)
        {
            double num_two = oper.GetReal();
            if (num_two == int(num_two))
            {
                c_two = to_string(int(num_two));
            }
            else
            {
                c_two = to_string(num_two);
                int count = c_two.size() - 1;
                while (count - 2 >= 0 && c_two[count - 2] != '.')
                {
                    c_two.pop_back();
                    count--;
                }
            }
        }
        else if (val_type_two == VINT)
        {
            c_two = to_string(oper.GetInt());
        }
        else
        {
            c_two = oper.GetString();
        }
        if (val_type_one == VREAL)
        {
            double number = GetReal();
            if (number == int(number))
            {
                return to_string((int(number))) + c_two;
            }
            else
            {
                string c_one = to_string(number);
                int x = c_one.size() - 1;
                while (x - 2 >= 0 && c_one[x - 2] != '.')
                {
                    c_one.pop_back();
                    x--;
                }
                return c_one + c_two;
            }
        }
        else if (val_type_one == VINT)
        {
            return to_string(GetInt()) + c_two;
        }
        else
        {
            return GetString() + c_two;
        }
    }
    // string repetition operation of this with op
    Value Repeat(const Value &oper) const
    {
       Value ans;
        if (oper.GetType() == VERR || GetType() == VERR)
        {
            return ans;
        }
        ValType val_type_one = GetType();
        ValType val_type_two = oper.GetType();
        string c_one, c_two;
        if (val_type_one == VSTRING)
        {
            c_one = GetString();
            if (val_type_two == VSTRING)
            {
                c_two = oper.GetString();
                if (valid(c_two))
                {
                    int lim = int(stod(c_two));
                    string ans;
                    for (int count = 0; count < lim; count++)
                    {
                     ans += c_one;
                    }
                    return ans;
                }
                else
                {
                    cout << "Invalid conversion from string to double\n";
                    return ans;
                }
            }
            else if (val_type_two == VINT)
            {
                string ans;
                int num_two = oper.GetInt();
                for (int count = 0; count < num_two; count++)
                {
                 ans += c_one;
                }
                return ans;
            }
            else
            {
                int num_two = (oper.GetReal());
                string ans;
                for (int count = 0; count < num_two; count++)
                {
                 ans+=c_one;
                }
                return ans;
            }
        }
        else if (val_type_one == VINT)
        {
            c_one = to_string(GetInt());
            if (val_type_two == VSTRING)
            {
                c_two = oper.GetString();
                return c_one < c_two;
            }
            else if (val_type_two == VINT)
            {
                return c_one == to_string(oper.GetInt());
            }
            else
            {
                c_two = to_string(oper.GetReal());
                int count = c_two.size() - 1;
                while (count - 2 >= 0 && c_two[count - 2] != '.')
                {
                    c_two.pop_back();
                    count--;
                }
                return c_one == c_two;
            }
        }
        else
        {
            c_one = to_string(GetReal());
            int count = c_one.size() - 1;
            while (count - 2 >= 0 && c_two[count - 2] != '.')
            {
                c_one.pop_back();
                count--;
            }
            if (val_type_two == VSTRING)
            {
                c_two = oper.GetString();

                return c_one < c_two;
            }
            else if (val_type_two == VINT)
            {
                return c_one == to_string(oper.GetInt());
            }
            else
            {
                c_two = to_string(oper.GetReal());
                int count = c_two.size() - 1;
                while (count - 2 >= 0 && c_two[count - 2] != '.')
                {
                    c_two.pop_back();
                    count--;
                }
                return c_one == c_two;
            }
        }
    }
    // string equality (-eq) operator of this with op
    Value SEqual(const Value &oper) const
    {
        const Value ans;
        if (T == VERR || oper.GetType() == VERR)
        {
            return ans;
        }
        ValType val_type_one = GetType();
        ValType val_type_two = oper.GetType();
        string c_one, c_two;
        if (val_type_one == VSTRING)
        {
            c_one = GetString();
            if (val_type_two == VSTRING)
            {
                c_two = oper.GetString();
                return c_one == c_two;
            }
            else if (val_type_two == VINT)
            {
                return c_one == to_string(oper.GetInt());
            }
            else
            {
                c_two = to_string(oper.GetReal());
                int count = c_two.size() - 1;
                while (count - 2 >= 0 && c_two[count - 2] != '.')
                {
                    c_two.pop_back();
                    count--;
                }
                return c_one == c_two;
            }
        }
        else if (val_type_one == VINT)
        {
            c_one = to_string(GetInt());
            if (val_type_two == VSTRING)
            {
                c_two = oper.GetString();
                return c_one < c_two;
            }
            else if (val_type_two == VINT)
            {
                return c_one == to_string(oper.GetInt());
            }
            else
            {
                c_two = to_string(oper.GetReal());
                int count = c_two.size() - 1;
                while (count - 2 >= 0 && c_two[count - 2] != '.')
                {
                    c_two.pop_back();
                    count--;
                }
                return c_one == c_two;
            }
        }
        else
        {
            c_one = to_string(GetReal());
            int count = c_one.size() - 1;
            while (count - 2 >= 0 && c_two[count - 2] != '.')
            {
                c_one.pop_back();
                count--;
            }
            if (val_type_two == VSTRING)
            {
                c_two = oper.GetString();

                return c_one < c_two;
            }
            else if (val_type_two == VINT)
            {
                return c_one == to_string(oper.GetInt());
            }
            else
            {
                c_two = to_string(oper.GetReal());
                int count = c_two.size() - 1;
                while (count - 2 >= 0 && c_two[count - 2] != '.')
                {
                    c_two.pop_back();
                    count--;
                }
                return c_one == c_two;
            }
        }
    }
   
	//string greater than (-gt) operator of this with op
	Value SGthan(const Value& oper) const{
        const Value ans;
        if (T == VERR || oper.GetType() == VERR)
        {
            return ans;
        }
        ValType val_type_one = GetType();
        ValType val_type_two = oper.GetType();
        string c_one, c_two;
        if (val_type_one == VSTRING)
        {
            c_one = GetString();
            if (val_type_two == VSTRING)
            {
                c_two = oper.GetString();
                return c_one > c_two;
            }
            else if (val_type_two == VINT)
            {
                return c_one > to_string(oper.GetInt());
            }
            else
            {
                c_two = to_string(oper.GetReal());
                int count = c_two.size() - 1;
                while (count - 2 >= 0 && c_two[count - 2] != '.')
                {
                    c_two.pop_back();
                    count--;
                }
                return c_one > c_two;
            }
        }
        else if (val_type_one == VINT)
        {
            c_one = to_string(GetInt());
            if (val_type_two == VSTRING)
            {
                c_two = oper.GetString();
                return c_one > c_two;
            }
            else if (val_type_two == VINT)
            {
                return c_one > to_string(oper.GetInt());
            }
            else
            {
                c_two = to_string(oper.GetReal());
                int count = c_two.size() - 1;
                while (count - 2 >= 0 && c_two[count - 2] != '.')
                {
                    c_two.pop_back();
                    count--;
                }
                return c_one > c_two;
            }
        }
        else
        {
            c_one = to_string(GetReal());
            int count = c_one.size() - 1;
            while (count - 2 >= 0 && c_two[count - 2] != '.')
            {
                c_one.pop_back();
                count--;
            }
            if (val_type_two == VSTRING)
            {
                c_two = oper.GetString();

                return c_one > c_two;
            }
            else if (val_type_two == VINT)
            {
                return c_one > to_string(oper.GetInt());
            }
            else
            {
                c_two = to_string(oper.GetReal());
                int count = c_two.size() - 1;
                while (count - 2 >= 0 && c_two[count - 2] != '.')
                {
                    c_two.pop_back();
                    count--;
                }
                return c_one > c_two;
            }
        }
    }
	
	Value SLthan(const Value& oper) const{
        const Value ans;
        if (T == VERR || oper.GetType() == VERR)
        {
            return ans;
        }
        ValType val_type_one = GetType();
        ValType val_type_two = oper.GetType();
        string c_one, c_two;
        if (val_type_one == VSTRING)
        {
            c_one = GetString();
            if (val_type_two == VSTRING)
            {
                c_two = oper.GetString();
                return c_one < c_two;
            }
            else if (val_type_two == VINT)
            {
                return c_one < to_string(oper.GetInt());
            }
            else
            {
                c_two = to_string(oper.GetReal());
                int count = c_two.size() - 1;
                while (count - 2 >= 0 && c_two[count - 2] != '.')
                {
                    c_two.pop_back();
                    count--;
                }
                return c_one < c_two;
            }
        }
        else if (val_type_one == VINT)
        {
            c_one = to_string(GetInt());
            if (val_type_two == VSTRING)
            {
                c_two = oper.GetString();
                return c_one < c_two;
            }
            else if (val_type_two == VINT)
            {
                return c_one < to_string(oper.GetInt());
            }
            else
            {
                c_two = to_string(oper.GetReal());
                int count = c_two.size() - 1;
                while (count - 2 >= 0 && c_two[count - 2] != '.')
                {
                    c_two.pop_back();
                    count--;
                }
                return c_one < c_two;
            }
        }
        else
        {
            c_one = to_string(GetReal());
            int count = c_one.size() - 1;
            while (count - 2 >= 0 && c_two[count - 2] != '.')
            {
                c_one.pop_back();
                count--;
            }
            if (val_type_two == VSTRING)
            {
                c_two = oper.GetString();

                return c_one < c_two;
            }
            else if (val_type_two == VINT)
            {
                return c_one < to_string(oper.GetInt());
            }
            else
            {
                c_two = to_string(oper.GetReal());
                int count = c_two.size() - 1;
                while (count - 2 >= 0 && c_two[count - 2] != '.')
                {
                    c_two.pop_back();
                    count--;
                }
                return c_one < c_two;
            }
        }
    }
	    
    friend ostream& operator<<(ostream& out, const Value& op) {
        if( op.IsInt() ) out << op.Itemp;
		else if( op.IsString() ) out << op.Stemp ;
        else if( op.IsReal()) out << fixed << showpoint << setprecision(1) << op.Rtemp;
        else if(op.IsBool()) out << (op.GetBool()? "true" : "false");
        else out << "ERROR";
        return out;
    }
};


#endif