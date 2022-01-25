#include "include/std_lib_facilities.h"

class Token {
public:
    char kind;        // what kind of token
    double value;     // for numbers: a value
    Token(char ch)    // make a Token from a char
            :kind(ch), value(0) { }
    Token(char ch, double val)     // make a Token from a char and a double
            :kind(ch), value(val) { }
};

class TokenStream {
public:
    TokenStream();
    Token get(); // get token
    void putBack(Token t); // put token back
private:
    bool full; // Is buffer full?
    Token buffer; // Buffer for token
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
    cin >> ch;    // note that >> skips whitespace (space, newline, tab, etc.)

    switch (ch) {
        case ';':    // for "print"
        case 'q':    // for "quit"
        case '(': case ')': case '+': case '-': case '*': case '/':
            return Token(ch);        // let each character represent itself
        case '.':
        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
        {
            cin.putback(ch);         // put digit back into the input stream
            double val;
            cin >> val;              // read a floating-point number
            return Token('8', val);   // let '8' represent "a number"
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
            return d;
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
    double val = 0;
    while (cin) {
        Token t = ts.get();
        if (t.kind == 'q') {
            // ‘q’ for “quit”
            break;
        }
        if (t.kind == ';') {
            // ‘;’ for “print now”
            cout << "=" << val << '\n';
        } else {
            ts.putBack(t);
            val = expression();
        }
    }
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