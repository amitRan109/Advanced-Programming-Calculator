#include<iostream>
#include<string>
#include<map>
#include<algorithm>
#include <regex>
#include<stdexcept>
using namespace std;

enum Tokens {
    NUM = 20, // actual number
    ID  = 21, // name of the variable
    INT = 22, // declaration of variable
    EOL = 23, // end of line
    IF  = 24,
    ELSE = 25   
};

class Token{
public:
    char kind;        	    // what kind of token
    int value;     	        // for numbers a value 
    string name;     	    // for variables a name 
    Token(char ch) :kind(ch), value(0) { }
    Token(char ch, int val) :kind(ch), value(val) { }
    Token(char ch, string n) :kind{ch}, name{n} { }
};

class Token_stream {
public:
    Token_stream() :full(false), buffer(0) {}
    Token get();            // get a Token
    void putback(Token t);  // put a Token back
private:
    bool full;              // is there a Token in the buffer?
    Token buffer;           // keep a Token when using putback()
};

// declarations so that functions can call each other
int statement();
int declaration();
int expression();
int term();
int primary();
int condition();
