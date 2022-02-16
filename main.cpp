/**
 * Simple command line calculator
 */

#include "include/std_lib_facilities.h"
#include "include/token.h"

// Class to represent variables
class Variable {
public:
    string name;
    double value;
    Variable(string n, double d) :name(n), value(d) { }
};

vector <Variable> varTable; // Vector containing declared variables
TokenStream ts;

// Get value of declared variable
double getValue(string s){
    for(const Variable& v : varTable) if(v.name == s) return v.value;
    error("get: undefined Variable!", s);
}

// Set value of declared value
void setValue(string s, double d){
    for(Variable& v: varTable){
        if(v.name == s){
            v.value = d;
            return;
        }
    }
    error("set: undefined Variable!");
}

double expression();

// Check if variable is declared
bool isDeclared(string name){
    for(const Variable& v: varTable){
        if(v.name == name) return true;
    }
    return false;
}

// Add new variable to varTable
double declareVar(string name, double value){
    if(isDeclared(name)) error(name, " declared twice");
    varTable.push_back(Variable(name,value));
    return value;
}

// Declaration of variable
double declaration(){
    Token t = ts.get();
    if(t.kind != name) error("name, expected in declaration!");
    string varName = t.name;

    Token t2 = ts.get();
    if(t2.kind != '=') error("missing '=' in declaration of ", varName);

    double d = expression();
    declareVar(varName, d);
    return d;
}

double statement() {
    Token t = ts.get();
    switch(t.kind){
        case let:
            return declaration();
        default:
        {
            ts.putBack(t);
            return expression();
        }
    }
}

void cleanup(){
    ts.ignore(print);
}

double primary() {
    Token t = ts.get();
    double d;
    if (isDeclared(t.name)) {
        return getValue(t.name);
    }
    switch(t.kind){
        case '(': {
            d = expression();
            t = ts.get();
            if (t.kind != ')') {
                cout << "Error ')' expected!";
            }
            return d;
        }
        case number:
            return t.value;
        case '-':
            return -primary();
        case '+':
            return +primary();
        default:
            error("primary expected");
    }
    return 0;
}

double term() {
    double left = primary();
    Token t = ts.get();
    while(true){
        switch(t.kind){
            case '*':
                left *= primary();
                t = ts.get();
                break;
            case '/':
                left /= primary();
                t = ts.get();
                break;
            case '%':
            {
                double d = primary();
                if (d == 0) error("divide by zero");
                left = fmod(left,d);
                t = ts.get();
                break;
            }
            default:
                ts.putBack(t);
                return left;
        }
    }
}

double expression() {
    double left = term();
    Token t = ts.get();
    while(true){
        switch(t.kind){
            case '+':
                left += term();
                t = ts.get();
                break;
            case '-':
                left -= term();
                t = ts.get();
                break;
            default:
                ts.putBack(t);
                return left;
        }
    }
}

void calculate () {
    while (cin)
    try {
        cout << "> ";
        Token t = ts.get();
        while (t.kind == print) t=ts.get();
        if (t.kind == quit) return;
        ts.putBack(t);
        cout << statement() << '\n';
    }
    catch (exception& e){
        cerr << e.what() << endl;
        cleanup();
    }
}

int main()
try {
    declareVar("pi", M_PI);
    declareVar("e", 2.71828182845904523536);

    cout << "type '" << print << "' after expression for output and '"<< quit <<"' to quit" << endl;

    calculate();
    return 0;
}
catch (runtime_error& e) {
    cerr << e.what() << '\n';
    return 1;
}
catch (exception& e) {
    cerr << e.what() << endl;
    return 1;
}
catch (...) {
    cerr << "exception \n";
    return 2;
}
