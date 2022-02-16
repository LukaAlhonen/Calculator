// Header file for class token

#include <string>

using namespace std;

// Definitions
const char number = '8'; // Number token kind
const char quit = 'q';
const char print = ';';
const char name = 'a'; // Variable token kind
const char let = 'L'; // Declaration of variable token kind
const string declKey = "let"; // Variable declaration keyword

class Token {
public:
    char kind; // what kind of token
    double value; // value of number token
    string name; // name of variable token
    // Constructors
    Token(char ch) :kind(ch) { } // init kind
    Token(char ch, double val) :kind(ch), value(val) { } // init with kind and value
    Token(char ch, string n) : kind(ch), name(n) { } // init with kind and name
};

// Class representing stream of tokens
class TokenStream {
public:
    TokenStream();
    Token get(); // Get token from stdin
    void putBack(Token t); // Put token in buffer
    void ignore(char c); // Ignore char c in buffer
private:
    bool full; // Is there a token in buffer?
    Token buffer; // Buffer to hold a token
};
