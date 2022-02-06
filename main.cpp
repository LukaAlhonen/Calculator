/**
 * Simple command line calculator
 */

// TODO: error handling

#include "include/std_lib_facilities.h"

const char number = '8';
const char quit = 'q';
const char print = '=';
const char name = 'a';
const char let = 'L';
const string declKey = 'let';

class Variable {
public:
    string name;
    double value;
};

class Token {
public:
    char kind;
    double value;
    Token(char ch)
            :kind(ch), value(0) { }
    Token(char ch, double val)
            :kind(ch), value(val) { }
};

class TokenStream {
public:
    TokenStream();
    Token get();
    void putBack(Token t);
    void ignore(char c);
private:
    bool full;
    Token buffer;
};

TokenStream::TokenStream()
    :full(false), buffer(0)
{
}

void TokenStream::putBack(Token t) {
    if(full){
        error("putback() into full buffer!");
    }
    buffer = t;
    full = true;
}

Token TokenStream::get() {
    if(full){
        full = false;
        return buffer;
    }

    char ch;
    cin >> ch;

    switch (ch) {
        case print:
        case quit:
        case '(':
        case ')':
        case '+':
        case '-':
        case '*':
        case '/':
        case '%':
            return Token(ch);
        case '.':
        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
        {
            cin.putback(ch);
            double val;
            cin >> val;
            return Token(number, val);
        }
        default:
            if(isalpha(ch)){
                cin.putback(ch);
                string s;
                cin >> s;
                if(s == declKey) return Token(let);
                return Token{name,s};
            }

            error("Bad token");
    }

}

void TokenStream::ignore(char c){
    if (full && c == buffer.kind){
        full = false;
        return;
    }
    full = false;

    char ch = 0;
    while(cin >> ch){
        if(ch == c) return;
    }
}

vector <Variable> varTable;
TokenStream ts;

double getValue(string s){
    for(const Variable& v : varTable) if(v.name == s) return v.value;
    error("get: undefined Variable!", s);
}

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

double primary() {
    Token t = ts.get();
    double d;
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

bool isDeclared(string name){
    for(const Variable& v: varTable){
        if(v.name == var) return true;
    }
    return false;
}

Variable declareVar(string name, double value){
    if(isDeclared(name)) error(name, " declared twice");
    varTable.push_back(Variable(name,value));
    return value;
}

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
            t.putBack();
            return expression();
        }
    }
}

void cleanup(){
    ts.ignore(print);
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
    cout << "type '=' after expression for output and 'q' to quit" << endl;
    calculate();
    keep_window_open();
    return 0;
}
catch (runtime_error& e) {
    cerr << e.what() << '\n';
    keep_window_open("~~");
    return 1;
}
catch (exception& e) {
    cerr << e.what() << endl;
    keep_window_open("~1");
    return 1;
}
catch (...) {
    cerr << "exception \n";
    keep_window_open("~2");
    return 2;
}
