#ifndef PARSER_H
#define PARSER_H

#include "token.h"

double expression(Token_stream &ts);
double term(Token_stream &ts);
double primary(Token_stream &ts);
double value(Token_stream &ts);
double statement(Token_stream &ts);
double declaration(Token_stream& ts);
double assignment(const std::string name, Token_stream &ts);

#endif
