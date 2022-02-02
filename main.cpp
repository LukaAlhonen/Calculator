/**
 * Simple command line calculator
 */

#include "include/std_lib_facilities.h"

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
        case '=':
        case 'x':
        case 'q':
        case '(': case ')': case '+': case '-': case '*': case '/':
            return Token(ch);
        case '.':
        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
        {
            cin.putback(ch);
            double val;
            cin >> val;
            return Token('8', val);
        }
        default:
            error("Bad token");
    }

}

TokenStream ts;

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
        case '8':
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

int main()
try {
    while (cin) {
        cout << "> ";
        Token t = ts.get();
        while (t.kind == '=') t=ts.get();
        if (t.kind == 'x' || t.kind == 'q') {
            keep_window_open();
            return 0;
        }
        ts.putBack(t);
        cout << expression() << '\n';
    }
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
