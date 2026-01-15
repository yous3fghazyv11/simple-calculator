#include "token.h"
#include <cctype>
#include <stdexcept>
#include <string>

void Token_stream::putback(Token t) {
    buffer.push_back(t);
}

Token Token_stream::get() {
    if (buffer.size() > 0) {
        Token last = buffer.back();
        buffer.pop_back();
        return last;
    }
    char ch;
    in >> ch;
    if (in.eof()) {              // stream ended
        return Token(Kind::eoe); // signal eoe, check for it in main
    }
    if (ch == '+') return Token(Kind::pls);
    if (ch == '-') return Token(Kind::mins);
    if (ch == '*') return Token(Kind::mul);
    if (ch == '/') return Token(Kind::div);
    if (ch == '^') return Token(Kind::pow);
    if (ch == '(') return Token(Kind::obrace);
    if (ch == ')') return Token(Kind::cbrace);
    if (ch == '!') return Token(Kind::fac);
    if (ch == '%') return Token(Kind::mod);
    if (ch == 'q') return Token(Kind::quit);
    if (ch == '=') return Token(Kind::eq);
    if (ch == '#') return Token(Kind::com);
    if (isdigit(ch) || ch == '.') { // case of a digit
        in.putback(ch);             // put back the character
        double val;
        in >> val; // try to read a double
        return Token(Kind::num, val);
    }
    std::string name;
    if (std::isalpha(static_cast<unsigned char>(ch)) || ch == '_') { // case of a name
        name += ch;
        while (std::isalpha(static_cast<unsigned char>(in.peek())) || in.peek() == '_') {
            in.get(ch);
            name += ch;
        }
        if (name == "let")
            return Token(Kind::dec);
        if (name == "quit")
            return Token(Kind::quit);
        return Token(Kind::name, name);
    }
    throw std::runtime_error("bad token: " + std::string(1, ch));
}
