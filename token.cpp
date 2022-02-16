#include "include/token.h"
#include "include/std_lib_facilities.h"

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
    // If there is a token in buffer return it
    if(full){
        full = false;
        return buffer;
    }

    // Read token with cin and check its kind
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
        case '=':
            return Token(ch);
        case '.':
        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
        {
            cin.putback(ch); // Put char back into input stream
            double val;
            cin >> val; // Read double from input stream
            return Token(number, val);
        }
        default:
            if(isalpha(ch)){
                string s;
                s += ch;
                while(cin.get(ch) && (isalpha(ch) || isdigit(ch))) s += ch;
                cin.putback(ch);
                if(s == declKey) {
                    return Token(let); // Variable declaration
                }

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
