#ifndef TOKEN_H
#define TOKEN_H

#include <iostream>
#include <istream>
#include <vector>

enum class Kind {
    num,
    pls = '+',
    mins = '-',
    mul = '*',
    div = '/',
    pow = '^',
    obrace = '(',
    cbrace = ')',
    quit = 'q',
    fac = '!',
    mod = '%',
    eq = '=',
    com = '#',
    name,
    dec,
    eoe // eoe means end of expression
    // signaled by Token_stream::get() and used in main to check garbage after an expression
};

struct Token {
    Kind kind;
    double value; // used only if kind == Kind::number
    std::string name; // used to represent tokens of type variable names
    Token(Kind k) : kind(k), value(0) {}
    Token(Kind k, double val) : kind(k), value(val) {}
    Token(Kind k, std::string n) : kind(k), value(0), name(n) {}
};

struct Token_stream {
    std::istream &in;      // a token stream tokenizes an input stream
    Token get();           // get a token from the stream
    void putback(Token t); // put a token back
    Token_stream(std::istream &s) : in(s), buffer({}), full(false) {}
    std::vector<Token> buffer;
    bool full;
};

#endif
