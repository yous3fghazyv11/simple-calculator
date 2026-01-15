#include "../include/parser.h"
#include "../include/token.h"
#include "../include/utils.h"
#include "../include/var.h"
#include <cmath>
#include <stdexcept>

double statement(Token_stream &ts)
/*
 * Statement:
 *   Declaration
 *   Assignment
 *   Expression
 */
{
    Token next = ts.get();
    switch (next.kind) {
        case Kind::dec: // checks for let, if yes, it's a declaration
            return declaration(ts);
        case Kind::name: { // checks for a name
            Token is_equal = ts.get();
            if (is_equal.kind == Kind::eq) {
                // checks for '=' after a name, if yes, it's an assignment
                return assignment(next.name, ts);
            }
            // if not an assignment, meaning that the name was not followed by
            // a '=', putback the name and the token that was supposed to be '='
            ts.putback(is_equal);
            ts.putback(next);
            return expression(ts);
        }
        default:
            // if not a declaration or assignment, try parse expression normally
            ts.putback(next);
            return expression(ts);
    }
}

double declaration(Token_stream &ts)
/*
 * Declaration:
 *   let name = Expression
 */
{
    Token next = ts.get();
    if (next.kind != Kind::name)
        throw std::runtime_error("expceted a name");
    if (ts.get().kind != Kind::eq)
        throw std::runtime_error("expected '='");
    if (is_defined(next.name))
        throw std::runtime_error("can't define more than once");
    return define_var(next.name, statement(ts));
}

double assignment(const std::string name, Token_stream &ts)
/*
 * Assignment:
 *   name = Expression
 */
{
    double val = statement(ts);
    set_value(name, val);
    return val;
}

double expression(Token_stream &ts)
/*
 * Expression:
 *   Term
 *   Term + Term*
 *   Term - Term*
 */
{
    double left = term(ts);
    Token next = ts.get();
    while (true) {
        switch (next.kind) {
            case Kind::pls:
                left += term(ts);
                next = ts.get();
                break;
            case Kind::mins:
                left -= term(ts);
                next = ts.get();
                break;
            default:
                ts.putback(next);
                return left;
        }
    }
}

double term(Token_stream &ts)
/*
 * Term:
 *   Primary
 *   Primary * Primary*
 *   Primary / Primary*
 *   Primary % Primary*
 *   Primary ^ Primary
 */
{
    double left = primary(ts);
    Token next = ts.get();
    while (true) {
        switch (next.kind) {
            case Kind::mul:
                left *= primary(ts);
                next = ts.get();
                break;
            case Kind::div: {
                double val = primary(ts);
                if (val == 0)
                    throw std::runtime_error("divison by zero");
                left /= val;
                next = ts.get();
                break;
            }
            case Kind::pow:
                return std::pow(left, primary(ts));
            default:
                ts.putback(next);
                return left;
        }
    }
}

double primary(Token_stream &ts)
/*
 * Primary:
 *   Value
 *   Value!
 *   Value%
 *   +Value
 *   -Value
 *   (Expression)
 *   (Expression)!
 */
{
    Token next = ts.get();
    switch (next.kind) {
        case Kind::obrace: {
            double inside = expression(ts);
            next = ts.get();
            if (next.kind != Kind::cbrace)
                throw std::runtime_error("')' expected");
            next = ts.get();
            if (next.kind == Kind::fac) {
                if (std::floor(inside) != inside) {
                    throw std::runtime_error("factorial of non-integer not allowed");
                }
                return factorial(inside);
            }
            else
                ts.putback(next);
            return inside;
        }
        case Kind::pls:
            return value(ts);
        case Kind::mins:
            return -1 * value(ts);
        default: {
            ts.putback(next);
            double num = value(ts);
            next = ts.get();
            if (next.kind == Kind::fac) {
                if (std::floor(num) != num) {
                    throw std::runtime_error("factorial of non-integer not allowed");
                }
                return factorial(num);
            }
			if (next.kind == Kind::mod) {
				return (num / 100);
			}
            ts.putback(next);
            return num;
        }
    }
}

double value(Token_stream &ts)
/*
 * Value:
 *   floating-point-value
 *   varibale name
 */
{
    Token next = ts.get();
    switch (next.kind) {
        case Kind::name:
            return get_value(next.name);
        case Kind::num:
            return next.value;
        default:
            throw std::runtime_error("invalid expression");
    }
}
