#include "../include/parser.h"
#include "../include/token.h"
#include "../include/var.h"
#include <exception>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <string>
#include <readline/readline.h>
#include <readline/history.h>

namespace color {
    constexpr char green[] = "\033[92m";
    constexpr char red[] =  "\033[33m";
    constexpr char white[] =  "\033[0m";
};

void repl() {
    std::cout << "type 'q' or 'quit' to exit\n";
    while (char *cline = readline("> ")) {
		if (!cline) {
			break;
		}
        if (*cline) add_history(cline);
        std::string line = cline;
        try {
            std::stringstream line_buffer{line};
            Token_stream ts(line_buffer);

            Token t = ts.get();
            if (t.kind == Kind::quit) {
                break;
            }
            if (t.kind == Kind::eoe) {
                continue;
            }
            if (t.kind == Kind::com) {
                continue;
            }
            ts.putback(t);
            double result = statement(ts);
            if (ts.get().kind != Kind::eoe) {
                std::cerr << color::red << "invalid expression\n" << color::green;
                continue;
            }
            set_value("ans", result);
            std::cout << color::green << result << color::white << '\n';
        } catch (std::exception &e) {
            std::cerr << color::red << e.what() << color::white << '\n';
        }
    }
}

void read_config() {
    std::string line;
    std::string config_path = std::string(std::getenv("HOME")) + "/.config/calc/calc.conf";
    std::ifstream file{config_path};
    if (!file) {
        std::cerr << "can't find config file in: " << config_path << '\n';
        return;
    }
    while (std::getline(file, line)) {
        try {
            std::stringstream line_buffer{line};
            Token_stream ts(line_buffer);
            Token t = ts.get();
            if (t.kind == Kind::com) {
                continue;
            } else if (t.kind == Kind::eoe) {
                continue;
            } else {
                ts.putback(t);
            }
            statement(ts);

            if (ts.get().kind != Kind::eoe) {
                std::cerr << color::red << "error in config file: " << config_path << color::white;
                std::cerr << color::red << "invalid expression: " << line << color::white;
                continue;
            }
        } catch (std::exception &e) {
            std::cerr << color::red << e.what() << color::white << '\n';
        }
    }
}

int main() try {
    define_var("ans", 0);
    read_config();
    repl();
    return 0;
} catch (std::exception &e) {
    std::cerr << color::red << "Error: " << e.what() << color::white << '\n';
    return 1;
} catch (...) {
    std::cerr << color::red << "Unexpected error accured" << color::white << '\n';
    return 2;
}
