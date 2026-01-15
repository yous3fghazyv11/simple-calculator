#include "var.h"
#include <stdexcept>

std::vector<Var> var_table;

double get_value(const std::string& s) {
    for (Var v : var_table) {
        if (v.name == s)
            return v.value;
    }
    throw std::runtime_error("undefined name: " + s);
}

void set_value(const std::string& s, double val) {
    for (Var& v : var_table) {
        if (v.name == s) {
            v.value = val;
            return;
        }
    }
    throw std::runtime_error("undefined name: " + s);
}

double define_var(const std::string& name, double val) {
    var_table.push_back(Var(name, val));
    return val;
}

bool is_defined(const std::string& name) {
    for (Var v : var_table) {
        if (v.name == name) {
            return true;
        }
    }
    return false;
}
